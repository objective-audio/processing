//
//  yas_processing_math_modules.cpp
//

#include "yas_processing_math_modules.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_buffer.h"
#include "yas_fast_each.h"

using namespace yas;

template <typename T>
processing::module processing::plus::make_signal_module() {
    auto left_buffer = make_buffer<T>(0, reserved_buffer_size);
    auto right_buffer = make_buffer<T>(0, reserved_buffer_size);
    auto left_time_range = std::make_shared<processing::time_range>();
    auto right_time_range = std::make_shared<processing::time_range>();

    auto receive_processor =
        processing::make_receive_signal_processor<T>([left_buffer, right_buffer, left_time_range, right_time_range](
            processing::time_range const &time_range, channel_index_t const, std::string const &key,
            T const *const signal_ptr) mutable {
            if (key == left_in_connector_key) {
                *left_time_range = time_range;
                auto const &length = time_range.length;
                auto &vec = get_vector<T>(left_buffer);
                vec.resize(length);
                memcpy(vec.data(), signal_ptr, length * sizeof(T));
            } else if (key == right_in_connector_key) {
                *right_time_range = time_range;
                auto const &length = time_range.length;
                auto &vec = get_vector<T>(right_buffer);
                vec.resize(length);
                memcpy(vec.data(), signal_ptr, length * sizeof(T));
            }
        });

    auto send_processor =
        processing::make_send_signal_processor<T>([left_buffer, right_buffer, left_time_range, right_time_range](
            processing::time_range const &time_range, channel_index_t const, std::string const &key,
            T *const signal_ptr) mutable {
            if (key == out_connector_key) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                auto const *left_ptr = get_data<T>(left_buffer);
                auto const *right_ptr = get_data<T>(right_buffer);
                auto const left_offset = time_range.start_frame - left_time_range->start_frame;
                auto const right_offset = time_range.start_frame - right_time_range->start_frame;
                auto const &left_length = left_time_range->length;
                auto const &right_length = right_time_range->length;
                
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

template processing::module processing::plus::make_signal_module<double>();
template processing::module processing::plus::make_signal_module<float>();
template processing::module processing::plus::make_signal_module<int64_t>();
template processing::module processing::plus::make_signal_module<int32_t>();
template processing::module processing::plus::make_signal_module<int16_t>();
template processing::module processing::plus::make_signal_module<int8_t>();
template processing::module processing::plus::make_signal_module<uint64_t>();
template processing::module processing::plus::make_signal_module<uint32_t>();
template processing::module processing::plus::make_signal_module<uint16_t>();
template processing::module processing::plus::make_signal_module<uint8_t>();
