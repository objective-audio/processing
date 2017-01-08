//
//  yas_processing_cast_module.cpp
//

#include "yas_processing_cast_module.h"

using namespace yas;

processing::cast::context::context(processing::signal_event &&signal) : signal(std::move(signal)) {
}

void processing::cast::context::reset() {
    signal.resize(0);
    time = nullptr;
}

template <typename T>
processing::cast::context_sptr processing::cast::make_context() {
    return std::make_shared<context>(make_signal_event<T>(0, reserved_signal_size));
}

template processing::cast::context_sptr processing::cast::make_context<double>();
template processing::cast::context_sptr processing::cast::make_context<float>();
template processing::cast::context_sptr processing::cast::make_context<int64_t>();
template processing::cast::context_sptr processing::cast::make_context<int32_t>();
template processing::cast::context_sptr processing::cast::make_context<int16_t>();
template processing::cast::context_sptr processing::cast::make_context<int8_t>();
template processing::cast::context_sptr processing::cast::make_context<uint64_t>();
template processing::cast::context_sptr processing::cast::make_context<uint32_t>();
template processing::cast::context_sptr processing::cast::make_context<uint16_t>();
template processing::cast::context_sptr processing::cast::make_context<uint8_t>();
