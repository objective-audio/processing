//
//  yas_processing_math1_modules.cpp
//

#include "yas_processing_math1_modules.h"
#include "yas_processing_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_send_number_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_receive_number_processor.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_constants.h"
#include "yas_processing_number_process_context.h"
#include "yas_processing_signal_process_context.h"
#include "yas_fast_each.h"

using namespace yas;

#pragma mark - signal

template <typename T>
processing::module processing::make_signal_module(math1::kind const kind) {
    using namespace yas::processing::math1;

    auto context = std::make_shared<signal_process_context<T, 1>>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(stream.sync_source().slice_length); };

    auto receive_processor = processing::make_receive_signal_processor<T>(
        [context](time::range const &time_range, sync_source const &, channel_index_t const,
                  connector_index_t const co_idx, T const *const signal_ptr) mutable {
            if (co_idx == to_connector_index(input::parameter)) {
                context->set_time(time{time_range}, co_idx);
                context->copy_data_from(signal_ptr, time_range.length, co_idx);
            }
        });

    auto send_processor = processing::make_send_signal_processor<T>(
        [context, kind](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                        connector_index_t const co_idx, T *const signal_ptr) {
            if (co_idx == to_connector_index(output::result)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                auto const input_co_idx = to_connector_index(input::parameter);
                T const *const input_ptr = context->data(input_co_idx);
                processing::time const &input_time = context->time(input_co_idx);
                auto const input_offset = input_time ? time_range.frame - input_time.get<time::range>().frame : 0;
                auto const &input_length = input_time ? input_time.get<time::range>().length : constant::zero_length;

                while (yas_fast_each_next(out_each)) {
                    auto const &idx = yas_fast_each_index(out_each);
                    auto const input_idx = idx + input_offset;

                    static T constexpr zero_value = 0;
                    auto const &input_value =
                        (input_idx >= 0 && input_idx < input_length) ? input_ptr[input_idx] : zero_value;

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

#pragma mark - number

template <typename T>
processing::module processing::make_number_module(math1::kind const kind) {
    using namespace yas::processing::math1;

    auto context = std::make_shared<number_process_context<T, 1>>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(); };

    auto receive_processor =
        make_receive_number_processor<T>([context](processing::time::frame::type const &frame, channel_index_t const,
                                                   connector_index_t const co_idx, T const &value) mutable {
            if (co_idx == to_connector_index(input::parameter)) {
                context->insert_input(frame, value, 0);
            }
        });

    auto send_processor =
        make_send_number_processor<T>([context, kind](processing::time::range const &, sync_source const &,
                                                      channel_index_t const, connector_index_t const co_idx) mutable {
            number_event::value_map_t<T> result;

            if (co_idx == to_connector_index(output::result)) {
                for (auto const &input_pair : context->inputs()) {
                    auto const &input_value = *input_pair.second.values[0];
                    T result_value = 0;

                    switch (kind) {
                        case kind::sin:
                            result_value = std::sin(input_value);
                            break;
                        case kind::cos:
                            result_value = std::cos(input_value);
                            break;
                        case kind::tan:
                            result_value = std::tan(input_value);
                            break;
                        case kind::asin:
                            result_value = std::asin(input_value);
                            break;
                        case kind::acos:
                            result_value = std::acos(input_value);
                            break;
                        case kind::atan:
                            result_value = std::atan(input_value);
                            break;

                        case kind::sinh:
                            result_value = std::sinh(input_value);
                            break;
                        case kind::cosh:
                            result_value = std::cosh(input_value);
                            break;
                        case kind::tanh:
                            result_value = std::tanh(input_value);
                            break;
                        case kind::asinh:
                            result_value = std::asinh(input_value);
                            break;
                        case kind::acosh:
                            result_value = std::acosh(input_value);
                            break;
                        case kind::atanh:
                            result_value = std::atanh(input_value);
                            break;

                        case kind::exp:
                            result_value = std::exp(input_value);
                            break;
                        case kind::exp2:
                            result_value = std::exp2(input_value);
                            break;
                        case kind::expm1:
                            result_value = std::expm1(input_value);
                            break;
                        case kind::log:
                            result_value = std::log(input_value);
                            break;
                        case kind::log10:
                            result_value = std::log10(input_value);
                            break;
                        case kind::log1p:
                            result_value = std::log1p(input_value);
                            break;
                        case kind::log2:
                            result_value = std::log2(input_value);
                            break;

                        case kind::sqrt:
                            result_value = std::sqrt(input_value);
                            break;
                        case kind::cbrt:
                            result_value = std::cbrt(input_value);
                            break;
                        case kind::abs:
                            result_value = std::abs(input_value);
                            break;

                        case kind::ceil:
                            result_value = std::ceil(input_value);
                            break;
                        case kind::floor:
                            result_value = std::floor(input_value);
                            break;
                        case kind::trunc:
                            result_value = std::trunc(input_value);
                            break;
                        case kind::round:
                            result_value = std::round(input_value);
                            break;
                    }

                    result.emplace(input_pair.first, result_value);
                }
            }

            return result;
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_number_module<double>(math1::kind const);
template processing::module processing::make_number_module<float>(math1::kind const);
