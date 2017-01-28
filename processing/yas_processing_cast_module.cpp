//
//  yas_processing_cast_module.cpp
//

#include "yas_processing_cast_module.h"

using namespace yas;

processing::cast::signal_context::signal_context(processing::signal_event &&signal) : signal(std::move(signal)) {
}

void processing::cast::signal_context::reset(std::size_t const reserve_size) {
    this->signal.reserve(reserve_size);
    this->signal.resize(0);
    this->time = nullptr;
}

template <typename T>
processing::cast::signal_context_sptr processing::cast::make_signal_context() {
    return std::make_shared<signal_context>(make_signal_event<T>(0));
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

template <typename T>
void processing::cast::number_context<T>::reset() {
    this->numbers.clear();
}

template void processing::cast::number_context<double>::reset();
template void processing::cast::number_context<float>::reset();
template void processing::cast::number_context<int64_t>::reset();
template void processing::cast::number_context<int32_t>::reset();
template void processing::cast::number_context<int16_t>::reset();
template void processing::cast::number_context<int8_t>::reset();
template void processing::cast::number_context<uint64_t>::reset();
template void processing::cast::number_context<uint32_t>::reset();
template void processing::cast::number_context<uint16_t>::reset();
template void processing::cast::number_context<uint8_t>::reset();

template <typename T>
processing::cast::number_context_sptr<T> processing::cast::make_number_context() {
    return std::make_shared<number_context<T>>();
}

template processing::cast::number_context_sptr<double> processing::cast::make_number_context<double>();
template processing::cast::number_context_sptr<float> processing::cast::make_number_context<float>();
template processing::cast::number_context_sptr<int64_t> processing::cast::make_number_context<int64_t>();
template processing::cast::number_context_sptr<int32_t> processing::cast::make_number_context<int32_t>();
template processing::cast::number_context_sptr<int16_t> processing::cast::make_number_context<int16_t>();
template processing::cast::number_context_sptr<int8_t> processing::cast::make_number_context<int8_t>();
template processing::cast::number_context_sptr<uint64_t> processing::cast::make_number_context<uint64_t>();
template processing::cast::number_context_sptr<uint32_t> processing::cast::make_number_context<uint32_t>();
template processing::cast::number_context_sptr<uint16_t> processing::cast::make_number_context<uint16_t>();
template processing::cast::number_context_sptr<uint8_t> processing::cast::make_number_context<uint8_t>();
