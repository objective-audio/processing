//
//  yas_processing_math1_modules.cpp
//

#include "yas_processing_math1_modules.h"
#include "yas_processing_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_constants.h"
#include "yas_fast_each.h"

using namespace yas;

namespace yas {
namespace processing {
    namespace math1 {
        template <typename T>
        static T constexpr zero_value = 0;

        struct context {
            signal_event input_signal;
            time input_time;

            context(signal_event &&input_signal) : input_signal(std::move(input_signal)) {
            }

            void reset(std::size_t const reserve_size) {
                this->input_signal.reserve(reserve_size);
                this->input_signal.resize(0);
                this->input_time = nullptr;
            }
        };

        using context_sptr = std::shared_ptr<context>;

        template <typename T>
        context_sptr make_context() {
            return std::make_shared<context>(make_signal_event<T>(0));
        }
    }
}
}

template <typename T>
processing::module processing::make_signal_module(math1::kind const kind) {
    using namespace yas::processing::math1;

    auto context = make_context<T>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(stream.sync_source().slice_length); };

    auto receive_processor = processing::make_receive_signal_processor<T>(
        [context](time::range const &time_range, sync_source const &, channel_index_t const,
                  connector_index_t const con_idx, T const *const signal_ptr) mutable {
            if (con_idx == to_connector_index(input::parameter)) {
                context->input_time = time_range;
                context->input_signal.copy_from(signal_ptr, time_range.length);
            }
        });

    auto send_processor = processing::make_send_signal_processor<T>(
        [context, kind](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                        connector_index_t const con_idx, T *const signal_ptr) {
            if (con_idx == to_connector_index(output::result)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                processing::signal_event &input_signal = context->input_signal;
                auto const *const input_ptr = input_signal.data<T>();
                processing::time const &input_time = context->input_time;
                auto const input_offset = input_time ? time_range.frame - input_time.get<time::range>().frame : 0;
                auto const &input_length = input_time ? input_time.get<time::range>().length : constant::zero_length;

                while (yas_fast_each_next(out_each)) {
                    auto const &idx = yas_fast_each_index(out_each);
                    auto const input_idx = idx + input_offset;
                    auto const &input_value =
                        (input_idx >= 0 && input_idx < input_length) ? input_ptr[input_idx] : zero_value<T>;

                    switch (kind) {
                        case kind::sin:
                            yas_fast_each_value(out_each) = std::sin(input_value);
                            break;
                        case kind::cos:
                            yas_fast_each_value(out_each) = std::cos(input_value);
                            break;
                        case kind::tan:
                            yas_fast_each_value(out_each) = std::tan(input_value);
                            break;
                        case kind::asin:
                            yas_fast_each_value(out_each) = std::asin(input_value);
                            break;
                        case kind::acos:
                            yas_fast_each_value(out_each) = std::acos(input_value);
                            break;
                        case kind::atan:
                            yas_fast_each_value(out_each) = std::atan(input_value);
                            break;

                        case kind::sinh:
                            yas_fast_each_value(out_each) = std::sinh(input_value);
                            break;
                        case kind::cosh:
                            yas_fast_each_value(out_each) = std::cosh(input_value);
                            break;
                        case kind::tanh:
                            yas_fast_each_value(out_each) = std::tanh(input_value);
                            break;
                        case kind::asinh:
                            yas_fast_each_value(out_each) = std::asinh(input_value);
                            break;
                        case kind::acosh:
                            yas_fast_each_value(out_each) = std::acosh(input_value);
                            break;
                        case kind::atanh:
                            yas_fast_each_value(out_each) = std::atanh(input_value);
                            break;

                        case kind::exp:
                            yas_fast_each_value(out_each) = std::exp(input_value);
                            break;
                        case kind::exp2:
                            yas_fast_each_value(out_each) = std::exp2(input_value);
                            break;
                        case kind::expm1:
                            yas_fast_each_value(out_each) = std::expm1(input_value);
                            break;
                        case kind::log:
                            yas_fast_each_value(out_each) = std::log(input_value);
                            break;
                        case kind::log10:
                            yas_fast_each_value(out_each) = std::log10(input_value);
                            break;
                        case kind::log1p:
                            yas_fast_each_value(out_each) = std::log1p(input_value);
                            break;
                        case kind::log2:
                            yas_fast_each_value(out_each) = std::log2(input_value);
                            break;

                        case kind::sqrt:
                            yas_fast_each_value(out_each) = std::sqrt(input_value);
                            break;
                        case kind::cbrt:
                            yas_fast_each_value(out_each) = std::cbrt(input_value);
                            break;
                        case kind::abs:
                            yas_fast_each_value(out_each) = std::abs(input_value);
                            break;

                        case kind::ceil:
                            yas_fast_each_value(out_each) = std::ceil(input_value);
                            break;
                        case kind::floor:
                            yas_fast_each_value(out_each) = std::floor(input_value);
                            break;
                        case kind::trunc:
                            yas_fast_each_value(out_each) = std::trunc(input_value);
                            break;
                        case kind::round:
                            yas_fast_each_value(out_each) = std::round(input_value);
                            break;
                    }
                }
            }
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_signal_module<double>(math1::kind const);
template processing::module processing::make_signal_module<float>(math1::kind const);
