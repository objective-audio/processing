//
//  yas_processing_math2_modules.cpp
//

#include "yas_processing_math2_modules.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_send_number_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_receive_number_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_number_event.h"
#include "yas_processing_constants.h"
#include "yas_processing_number_process_context.h"
#include "yas_processing_signal_process_context.h"
#include "yas_fast_each.h"

using namespace yas;

#pragma mark - signal

template <typename T>
processing::module processing::make_signal_module(math2::kind const kind) {
    using namespace yas::processing::math2;

    auto context = std::make_shared<signal_process_context<T, 2>>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(stream.sync_source().slice_length); };

    auto receive_processor = processing::make_receive_signal_processor<T>(
        [context](time::range const &time_range, sync_source const &, channel_index_t const,
                  connector_index_t const co_idx, T const *const signal_ptr) mutable {
            if (co_idx == to_connector_index(input::left) || co_idx == to_connector_index(input::right)) {
                context->set_time(time{time_range}, co_idx);
                context->copy_data_from(signal_ptr, time_range.length, co_idx);
            }
        });

    auto send_processor = processing::make_send_signal_processor<T>(
        [context, kind](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                        connector_index_t const co_idx, T *const signal_ptr) {
            if (co_idx == to_connector_index(output::result)) {
                static auto const left_co_idx = to_connector_index(input::left);
                static auto const right_co_idx = to_connector_index(input::right);

                auto const *left_ptr = context->data(left_co_idx);
                auto const *right_ptr = context->data(right_co_idx);
                processing::time const &left_time = context->time(left_co_idx);
                processing::time const &right_time = context->time(right_co_idx);
                auto const left_offset = left_time ? time_range.frame - left_time.get<time::range>().frame : 0;
                auto const right_offset = right_time ? time_range.frame - right_time.get<time::range>().frame : 0;
                auto const &left_length = left_time ? left_time.get<time::range>().length : constant::zero_length;
                auto const &right_length = right_time ? right_time.get<time::range>().length : constant::zero_length;

                auto out_each = make_fast_each(signal_ptr, time_range.length);
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

                        case kind::atan2:
                            yas_fast_each_value(out_each) = std::atan2(left_value, right_value);
                            break;

                        case kind::pow:
                            yas_fast_each_value(out_each) = std::pow(left_value, right_value);
                            break;
                        case kind::hypot:
                            yas_fast_each_value(out_each) = std::hypot(left_value, right_value);
                            break;
                    }
                }
            }
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_signal_module<double>(math2::kind const);
template processing::module processing::make_signal_module<float>(math2::kind const);
template processing::module processing::make_signal_module<int64_t>(math2::kind const);
template processing::module processing::make_signal_module<int32_t>(math2::kind const);
template processing::module processing::make_signal_module<int16_t>(math2::kind const);
template processing::module processing::make_signal_module<int8_t>(math2::kind const);
template processing::module processing::make_signal_module<uint64_t>(math2::kind const);
template processing::module processing::make_signal_module<uint32_t>(math2::kind const);
template processing::module processing::make_signal_module<uint16_t>(math2::kind const);
template processing::module processing::make_signal_module<uint8_t>(math2::kind const);

#pragma mark - number

template <typename T>
processing::module processing::make_number_module(math2::kind const kind) {
    using namespace yas::processing::math2;

    auto context = std::make_shared<number_process_context<T, 2>>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(); };

    auto receive_processor =
        make_receive_number_processor<T>([context](processing::time::frame::type const &frame, channel_index_t const,
                                                   connector_index_t const co_idx, T const &value) mutable {
            if (co_idx == to_connector_index(input::left)) {
                context->insert_input(frame, value, to_connector_index(input::left));
            } else if (co_idx == to_connector_index(input::right)) {
                context->insert_input(frame, value, to_connector_index(input::right));
            }
        });

    auto send_processor =
        make_send_number_processor<T>([context, kind](processing::time::range const &, sync_source const &,
                                                      channel_index_t const, connector_index_t const co_idx) mutable {
            number_event::value_map_t<T> result;
            T const *last_values = context->last_values();

            if (co_idx == to_connector_index(output::result)) {
                static auto const left_co_idx = to_connector_index(input::left);
                static auto const right_co_idx = to_connector_index(input::right);

                for (auto const &input_pair : context->inputs()) {
                    context->update_last_values(input_pair.second);
                    T const &left_value = last_values[left_co_idx];
                    T const &right_value = last_values[right_co_idx];
                    T result_value;

                    switch (kind) {
                        case kind::plus:
                            result_value = left_value + right_value;
                            break;
                        case kind::minus:
                            result_value = left_value - right_value;
                            break;
                        case kind::multiply:
                            result_value = left_value * right_value;
                            break;
                        case kind::divide:
                            result_value = (left_value == 0 || right_value == 0) ? 0 : left_value / right_value;
                            break;

                        case kind::atan2:
                            result_value = std::atan2(left_value, right_value);
                            break;

                        case kind::pow:
                            result_value = std::pow(left_value, right_value);
                            break;
                        case kind::hypot:
                            result_value = std::hypot(left_value, right_value);
                            break;
                    }

                    result.emplace(input_pair.first, result_value);
                }
            }

            return result;
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_number_module<double>(math2::kind const);
template processing::module processing::make_number_module<float>(math2::kind const);
template processing::module processing::make_number_module<int64_t>(math2::kind const);
template processing::module processing::make_number_module<int32_t>(math2::kind const);
template processing::module processing::make_number_module<int16_t>(math2::kind const);
template processing::module processing::make_number_module<int8_t>(math2::kind const);
template processing::module processing::make_number_module<uint64_t>(math2::kind const);
template processing::module processing::make_number_module<uint32_t>(math2::kind const);
template processing::module processing::make_number_module<uint16_t>(math2::kind const);
template processing::module processing::make_number_module<uint8_t>(math2::kind const);
