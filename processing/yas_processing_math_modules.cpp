//
//  yas_processing_math_modules.cpp
//

#include "yas_processing_math_modules.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_signal_event.h"
#include "yas_fast_each.h"

using namespace yas;

namespace yas {
namespace processing {
    namespace math {
        struct context {
            signal_event left_signal;
            signal_event right_signal;
            time left_time;
            time right_time;

            context(signal_event &&left_signal, signal_event &&right_signal)
                : left_signal(std::move(left_signal)), right_signal(std::move(right_signal)) {
            }

            void reset(std::size_t const reserve_size) {
                this->left_signal.reserve(reserve_size);
                this->right_signal.reserve(reserve_size);
                this->left_signal.resize(0);
                this->right_signal.resize(0);
                this->left_time = nullptr;
                this->right_time = nullptr;
            }
        };

        using context_sptr = std::shared_ptr<context>;

        template <typename T>
        context_sptr make_context() {
            return std::make_shared<context>(make_signal_event<T>(0), make_signal_event<T>(0));
        }

        processor_f make_prepare_processor(context_sptr &context) {
            return [context](time::range const &, connector_map_t const &, connector_map_t const &, stream &stream) mutable {
                context->reset(stream.sync_source().slice_length);
            };
        }

        template <typename T>
        processor_f make_receive_signal_processor(context_sptr const &context) {
            return processing::make_receive_signal_processor<T>(
                [context](time::range const &time_range, sync_source const &, channel_index_t const,
                          connector_index_t const con_idx, T const *const signal_ptr) mutable {
                    if (con_idx == to_connector_index(input_key::left)) {
                        context->left_time = time_range;
                        context->left_signal.copy_from(signal_ptr, time_range.length);
                    } else if (con_idx == to_connector_index(input_key::right)) {
                        context->right_time = time_range;
                        context->right_signal.copy_from(signal_ptr, time_range.length);
                    }
                });
        }
    }
}
}

template <typename T>
processing::module processing::make_signal_module(math::kind const kind) {
    using namespace yas::processing::math;

    auto context = make_context<T>();

    auto prepare_processor = make_prepare_processor(context);

    auto receive_processor = make_receive_signal_processor<T>(context);

    auto send_processor = processing::make_send_signal_processor<T>(
        [context, kind](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                        connector_index_t const con_idx, T *const signal_ptr) {
            if (con_idx == to_connector_index(output_key::out)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                processing::signal_event &left_signal = context->left_signal;
                processing::signal_event &right_signal = context->right_signal;
                auto const *left_ptr = left_signal.data<T>();
                auto const *right_ptr = right_signal.data<T>();
                processing::time const &left_time = context->left_time;
                processing::time const &right_time = context->right_time;
                auto const left_offset = left_time ? time_range.frame - left_time.get<time::range>().frame : 0;
                auto const right_offset = right_time ? time_range.frame - right_time.get<time::range>().frame : 0;
                auto const &left_length = left_time ? left_time.get<time::range>().length : zero_length;
                auto const &right_length = right_time ? right_time.get<time::range>().length : zero_length;

                while (yas_fast_each_next(out_each)) {
                    auto const &idx = yas_fast_each_index(out_each);
                    auto const left_idx = idx + left_offset;
                    auto const right_idx = idx + right_offset;
                    auto const &left_value = (left_idx >= 0 && left_idx < left_length) ? left_ptr[left_idx] : 0;
                    auto const &right_value = (right_idx >= 0 && right_idx < right_length) ? right_ptr[right_idx] : 0;

                    switch (kind) {
                        case kind::plus:
                            yas_fast_each_value(out_each) = left_value + right_value;
                            break;
                        case kind::minus:
                            yas_fast_each_value(out_each) = left_value - right_value;
                            break;
                        case kind::multiply:
                            yas_fast_each_value(out_each) = left_value * right_value;
                            break;
                        case kind::divide:
                            yas_fast_each_value(out_each) =
                                (left_value == 0 || right_value == 0) ? 0 : left_value / right_value;
                            break;
                    }
                }
            }
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_signal_module<double>(math::kind const);
template processing::module processing::make_signal_module<float>(math::kind const);
template processing::module processing::make_signal_module<int64_t>(math::kind const);
template processing::module processing::make_signal_module<int32_t>(math::kind const);
template processing::module processing::make_signal_module<int16_t>(math::kind const);
template processing::module processing::make_signal_module<int8_t>(math::kind const);
template processing::module processing::make_signal_module<uint64_t>(math::kind const);
template processing::module processing::make_signal_module<uint32_t>(math::kind const);
template processing::module processing::make_signal_module<uint16_t>(math::kind const);
template processing::module processing::make_signal_module<uint8_t>(math::kind const);
