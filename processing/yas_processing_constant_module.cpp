//
//  yas_processing_constant_module.cpp
//

#include "yas_processing_constant_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_module.h"
#include "yas_fast_each.h"
#include "yas_boolean.h"

using namespace yas;

template <typename T>
processing::module processing::make_signal_module(T value) {
    auto processor = processing::make_send_signal_processor<T>([value = std::move(value)](
        processing::time::range const &time_range, sync_source const &, channel_index_t const, connector_index_t const,
        T *const signal_ptr) {
        auto each = make_fast_each(signal_ptr, time_range.length);

        while (yas_fast_each_next(each)) {
            yas_fast_each_value(each) = value;
        }
    });

    return processing::module{{std::move(processor)}};
}

template processing::module processing::make_signal_module(double);
template processing::module processing::make_signal_module(float);
template processing::module processing::make_signal_module(int64_t);
template processing::module processing::make_signal_module(int32_t);
template processing::module processing::make_signal_module(int16_t);
template processing::module processing::make_signal_module(int8_t);
template processing::module processing::make_signal_module(uint64_t);
template processing::module processing::make_signal_module(uint32_t);
template processing::module processing::make_signal_module(uint16_t);
template processing::module processing::make_signal_module(uint8_t);
template processing::module processing::make_signal_module(boolean);
