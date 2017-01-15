//
//  yas_processing_constant_module.cpp
//

#include "yas_processing_constant_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_module.h"
#include "yas_fast_each.h"

using namespace yas;

template <typename T>
processing::module processing::constant::make_signal_module(T value) {
    auto processor = processing::make_send_signal_processor<T>([value = std::move(value)](
        processing::time::range const &time_range, channel_index_t const, connector_index_t const,
        T *const signal_ptr) {
        auto each = make_fast_each(signal_ptr, time_range.length);

        while (yas_fast_each_next(each)) {
            yas_fast_each_value(each) = value;
        }
    });

    return processing::module{{std::move(processor)}};
}

template processing::module processing::constant::make_signal_module<double>(double);
template processing::module processing::constant::make_signal_module<float>(float);
template processing::module processing::constant::make_signal_module<int64_t>(int64_t);
template processing::module processing::constant::make_signal_module<int32_t>(int32_t);
template processing::module processing::constant::make_signal_module<int16_t>(int16_t);
template processing::module processing::constant::make_signal_module<int8_t>(int8_t);
template processing::module processing::constant::make_signal_module<uint64_t>(uint64_t);
template processing::module processing::constant::make_signal_module<uint32_t>(uint32_t);
template processing::module processing::constant::make_signal_module<uint16_t>(uint16_t);
template processing::module processing::constant::make_signal_module<uint8_t>(uint8_t);
