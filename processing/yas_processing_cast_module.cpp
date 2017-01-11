//
//  yas_processing_cast_module.cpp
//

#include "yas_processing_cast_module.h"

using namespace yas;

processing::cast::signal_context::signal_context(processing::signal_event &&signal) : signal(std::move(signal)) {
}

void processing::cast::signal_context::reset() {
    signal.resize(0);
    time = nullptr;
}

template <typename T>
processing::cast::signal_context_sptr processing::cast::make_signal_context() {
    return std::make_shared<signal_context>(make_signal_event<T>(0, reserved_signal_size));
}

template processing::cast::signal_context_sptr processing::cast::make_signal_context<double>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<float>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<int64_t>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<int32_t>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<int16_t>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<int8_t>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<uint64_t>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<uint32_t>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<uint16_t>();
template processing::cast::signal_context_sptr processing::cast::make_signal_context<uint8_t>();
