//
//  yas_processing_math_modules.cpp
//

#include "yas_processing_math_modules.h"
#include "yas_processing_notify_processor.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_buffer.h"
#include "yas_fast_each.h"

using namespace yas;

namespace yas {
namespace processing {
    namespace math {
        struct context {
            buffer left_buffer;
            buffer right_buffer;
            time left_time;
            time right_time;

            context(buffer &&left_buffer, buffer &&right_buffer)
                : left_buffer(std::move(left_buffer)), right_buffer(std::move(right_buffer)) {
            }

            void reset() {
                left_buffer.resize(0);
                right_buffer.resize(0);
                left_time = nullptr;
                right_time = nullptr;
            }
        };

        using context_sptr = std::shared_ptr<context>;

        template <typename T>
        context_sptr make_context() {
            return std::make_shared<context>(make_buffer<T>(0, reserved_buffer_size),
                                             make_buffer<T>(0, reserved_buffer_size));
        }

        processor make_prepare_processor(context_sptr &context) {
            return processing::make_notify_processor(
                [context](processing::time::range const &) mutable { context->reset(); });
        }

        template <typename T>
        processor make_receive_signal_processor(context_sptr const &context) {
            return processing::make_receive_signal_processor<T>([context](processing::time::range const &time_range,
                                                                          channel_index_t const, std::string const &key,
                                                                          T const *const signal_ptr) mutable {
                if (key == left_in_connector_key) {
                    context->left_time = time_range;
                    auto const &length = time_range.length;
                    context->left_buffer.resize(length);
                    memcpy(context->left_buffer.data<T>(), signal_ptr, length * sizeof(T));
                } else if (key == right_in_connector_key) {
                    context->right_time = time_range;
                    auto const &length = time_range.length;
                    context->right_buffer.resize(length);
                    memcpy(context->right_buffer.data<T>(), signal_ptr, length * sizeof(T));
                }
            });
        }
    }
}
}

template <typename T>
processing::module processing::math::make_plus_signal_module() {
    auto context = make_context<T>();

    auto receive_processor = make_receive_signal_processor<T>(context);

    auto send_processor = processing::make_send_signal_processor<T>([context](
        processing::time::range const &time_range, channel_index_t const, std::string const &key, T *const signal_ptr) {
        if (key == out_connector_key) {
            auto out_each = make_fast_each(signal_ptr, time_range.length);
            processing::buffer &left_buffer = context->left_buffer;
            processing::buffer &right_buffer = context->right_buffer;
            auto const *left_ptr = left_buffer.data<T>();
            auto const *right_ptr = right_buffer.data<T>();
            processing::time const &left_time = context->left_time;
            processing::time const &right_time = context->right_time;
            auto const left_offset = left_time ? time_range.frame - left_time.get<time::range>().frame : 0;
            auto const right_offset = right_time ? time_range.frame - right_time.get<time::range>().frame : 0;
            auto const &left_length = left_time ? left_time.get<time::range>().length : 0;
            auto const &right_length = right_time ? right_time.get<time::range>().length : 0;

            while (yas_fast_each_next(out_each)) {
                auto const &idx = yas_fast_each_index(out_each);
                auto const left_idx = idx + left_offset;
                auto const right_idx = idx + right_offset;
                auto const &left_value = (left_idx >= 0 && left_idx < left_length) ? left_ptr[left_idx] : 0;
                auto const &right_value = (right_idx >= 0 && right_idx < right_length) ? right_ptr[right_idx] : 0;
                yas_fast_each_value(out_each) = left_value + right_value;
            }
        }
    });

    return processing::module{{std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::math::make_plus_signal_module<double>();
template processing::module processing::math::make_plus_signal_module<float>();
template processing::module processing::math::make_plus_signal_module<int64_t>();
template processing::module processing::math::make_plus_signal_module<int32_t>();
template processing::module processing::math::make_plus_signal_module<int16_t>();
template processing::module processing::math::make_plus_signal_module<int8_t>();
template processing::module processing::math::make_plus_signal_module<uint64_t>();
template processing::module processing::math::make_plus_signal_module<uint32_t>();
template processing::module processing::math::make_plus_signal_module<uint16_t>();
template processing::module processing::math::make_plus_signal_module<uint8_t>();

template <typename T>
processing::module processing::math::make_minus_signal_module() {
    auto context = make_context<T>();

    auto receive_processor = make_receive_signal_processor<T>(context);

    auto send_processor = processing::make_send_signal_processor<T>([context](
        processing::time::range const &time_range, channel_index_t const, std::string const &key, T *const signal_ptr) {
        if (key == out_connector_key) {
            auto out_each = make_fast_each(signal_ptr, time_range.length);
            processing::buffer &left_buffer = context->left_buffer;
            processing::buffer &right_buffer = context->right_buffer;
            auto const *left_ptr = left_buffer.data<T>();
            auto const *right_ptr = right_buffer.data<T>();
            processing::time const &left_time = context->left_time;
            processing::time const &right_time = context->right_time;
            auto const left_offset = left_time ? time_range.frame - left_time.get<time::range>().frame : 0;
            auto const right_offset = right_time ? time_range.frame - right_time.get<time::range>().frame : 0;
            auto const &left_length = left_time ? left_time.get<time::range>().length : 0;
            auto const &right_length = right_time ? right_time.get<time::range>().length : 0;

            while (yas_fast_each_next(out_each)) {
                auto const &idx = yas_fast_each_index(out_each);
                auto const left_idx = idx + left_offset;
                auto const right_idx = idx + right_offset;
                auto const &left_value = (left_idx >= 0 && left_idx < left_length) ? left_ptr[left_idx] : 0;
                auto const &right_value = (right_idx >= 0 && right_idx < right_length) ? right_ptr[right_idx] : 0;
                yas_fast_each_value(out_each) = left_value - right_value;
            }
        }
    });

    return processing::module{{std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::math::make_minus_signal_module<double>();
template processing::module processing::math::make_minus_signal_module<float>();
template processing::module processing::math::make_minus_signal_module<int64_t>();
template processing::module processing::math::make_minus_signal_module<int32_t>();
template processing::module processing::math::make_minus_signal_module<int16_t>();
template processing::module processing::math::make_minus_signal_module<int8_t>();
template processing::module processing::math::make_minus_signal_module<uint64_t>();
template processing::module processing::math::make_minus_signal_module<uint32_t>();
template processing::module processing::math::make_minus_signal_module<uint16_t>();
template processing::module processing::math::make_minus_signal_module<uint8_t>();

template <typename T>
processing::module processing::math::make_multiply_signal_module() {
    auto context = make_context<T>();

    auto receive_processor = make_receive_signal_processor<T>(context);

    auto send_processor = processing::make_send_signal_processor<T>([context](
        processing::time::range const &time_range, channel_index_t const, std::string const &key, T *const signal_ptr) {
        if (key == out_connector_key) {
            auto out_each = make_fast_each(signal_ptr, time_range.length);
            processing::buffer &left_buffer = context->left_buffer;
            processing::buffer &right_buffer = context->right_buffer;
            auto const *left_ptr = left_buffer.data<T>();
            auto const *right_ptr = right_buffer.data<T>();
            processing::time const &left_time = context->left_time;
            processing::time const &right_time = context->right_time;
            auto const left_offset = left_time ? time_range.frame - left_time.get<time::range>().frame : 0;
            auto const right_offset = right_time ? time_range.frame - right_time.get<time::range>().frame : 0;
            auto const &left_length = left_time ? left_time.get<time::range>().length : 0;
            auto const &right_length = right_time ? right_time.get<time::range>().length : 0;

            while (yas_fast_each_next(out_each)) {
                auto const &idx = yas_fast_each_index(out_each);
                auto const left_idx = idx + left_offset;
                auto const right_idx = idx + right_offset;
                auto const &left_value = (left_idx >= 0 && left_idx < left_length) ? left_ptr[left_idx] : 0;
                auto const &right_value = (right_idx >= 0 && right_idx < right_length) ? right_ptr[right_idx] : 0;
                yas_fast_each_value(out_each) = left_value * right_value;
            }
        }
    });

    return processing::module{{std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::math::make_multiply_signal_module<double>();
template processing::module processing::math::make_multiply_signal_module<float>();
template processing::module processing::math::make_multiply_signal_module<int64_t>();
template processing::module processing::math::make_multiply_signal_module<int32_t>();
template processing::module processing::math::make_multiply_signal_module<int16_t>();
template processing::module processing::math::make_multiply_signal_module<int8_t>();
template processing::module processing::math::make_multiply_signal_module<uint64_t>();
template processing::module processing::math::make_multiply_signal_module<uint32_t>();
template processing::module processing::math::make_multiply_signal_module<uint16_t>();
template processing::module processing::math::make_multiply_signal_module<uint8_t>();

template <typename T>
processing::module processing::math::make_divide_signal_module() {
    auto context = make_context<T>();

    auto receive_processor = make_receive_signal_processor<T>(context);

    auto send_processor = processing::make_send_signal_processor<T>([context](
        processing::time::range const &time_range, channel_index_t const, std::string const &key, T *const signal_ptr) {
        if (key == out_connector_key) {
            auto out_each = make_fast_each(signal_ptr, time_range.length);
            processing::buffer &left_buffer = context->left_buffer;
            processing::buffer &right_buffer = context->right_buffer;
            auto const *left_ptr = left_buffer.data<T>();
            auto const *right_ptr = right_buffer.data<T>();
            processing::time const &left_time = context->left_time;
            processing::time const &right_time = context->right_time;
            auto const left_offset = left_time ? time_range.frame - left_time.get<time::range>().frame : 0;
            auto const right_offset = right_time ? time_range.frame - right_time.get<time::range>().frame : 0;
            auto const &left_length = left_time ? left_time.get<time::range>().length : 0;
            auto const &right_length = right_time ? right_time.get<time::range>().length : 0;

            while (yas_fast_each_next(out_each)) {
                auto const &idx = yas_fast_each_index(out_each);
                auto const left_idx = idx + left_offset;
                auto const right_idx = idx + right_offset;
                auto const &left_value = (left_idx >= 0 && left_idx < left_length) ? left_ptr[left_idx] : 0;
                auto const &right_value = (right_idx >= 0 && right_idx < right_length) ? right_ptr[right_idx] : 0;
                yas_fast_each_value(out_each) = (left_value == 0 || right_value == 0) ? 0 : left_value / right_value;
            }
        }
    });

    return processing::module{{std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::math::make_divide_signal_module<double>();
template processing::module processing::math::make_divide_signal_module<float>();
template processing::module processing::math::make_divide_signal_module<int64_t>();
template processing::module processing::math::make_divide_signal_module<int32_t>();
template processing::module processing::math::make_divide_signal_module<int16_t>();
template processing::module processing::math::make_divide_signal_module<int8_t>();
template processing::module processing::math::make_divide_signal_module<uint64_t>();
template processing::module processing::math::make_divide_signal_module<uint32_t>();
template processing::module processing::math::make_divide_signal_module<uint16_t>();
template processing::module processing::math::make_divide_signal_module<uint8_t>();
