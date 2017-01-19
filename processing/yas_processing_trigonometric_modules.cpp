//
//  yas_processing_trigonometric_modules.cpp
//

#include "yas_processing_trigonometric_modules.h"
#include "yas_processing_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_signal_event.h"
#include "yas_fast_each.h"

using namespace yas;

namespace yas {
namespace processing {
    namespace trigonometric {
        template <typename T>
        static T constexpr zero_value = 0;

        struct context {
            signal_event phase_signal;
            time phase_time;

            context(signal_event &&phase_signal) : phase_signal(std::move(phase_signal)) {
            }

            void reset() {
                phase_signal.resize(0);
                phase_time = nullptr;
            }
        };

        using context_sptr = std::shared_ptr<context>;

        template <typename T>
        context_sptr make_context() {
            return std::make_shared<context>(make_signal_event<T>(0, reserved_signal_size));
        }
    }
}
}

template <typename T>
processing::module processing::make_signal_module(trigonometric::kind const kind) {
    using namespace yas::processing::trigonometric;

    auto context = make_context<T>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &) mutable { context->reset(); };

    auto receive_processor = processing::make_receive_signal_processor<T>(
        [context](processing::time::range const &time_range, channel_index_t const, connector_index_t const con_idx,
                  T const *const signal_ptr) mutable {
            if (con_idx == to_connector_index(input_key::phase)) {
                context->phase_time = time_range;
                context->phase_signal.copy_from(signal_ptr, time_range.length);
            }
        });

    auto send_processor = processing::make_send_signal_processor<T>(
        [context, kind](processing::time::range const &time_range, channel_index_t const,
                        connector_index_t const con_idx, T *const signal_ptr) {
            if (con_idx == to_connector_index(output_key::out)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                processing::signal_event &phase_signal = context->phase_signal;
                auto const *phase_ptr = phase_signal.data<T>();
                processing::time const &phase_time = context->phase_time;
                auto const phase_offset = phase_time ? time_range.frame - phase_time.get<time::range>().frame : 0;
                auto const &phase_length = phase_time ? phase_time.get<time::range>().length : zero_length;

                while (yas_fast_each_next(out_each)) {
                    auto const &idx = yas_fast_each_index(out_each);
                    auto const phase_idx = idx + phase_offset;
                    auto const &phase_value =
                        (phase_idx >= 0 && phase_idx < phase_length) ? phase_ptr[phase_idx] : zero_value<T>;

                    switch (kind) {
                        case kind::sin:
                            yas_fast_each_value(out_each) = std::sin(phase_value);
                            break;
                    }
                }
            }
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_signal_module<double>(trigonometric::kind const);
template processing::module processing::make_signal_module<float>(trigonometric::kind const);
