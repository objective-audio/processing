//
//  yas_processing_signal_generator_module.cpp
//

#include "yas_processing_signal_generator_module.h"

using namespace yas;

processing::signal_generator_module::signal_generator_module(std::shared_ptr<signal_generator_module::impl_base> &&impl)
    : module(std::move(impl)) {
}

processing::signal_generator_module::signal_generator_module(std::nullptr_t) : module(nullptr) {
}

template <typename T>
processing::signal_generator_module processing::make_signal_generator_module(signal_generator_module::args<T> args) {
    return signal_generator_module{std::make_shared<signal_generator_module::impl<T>>(std::move(args))};
}

template processing::signal_generator_module processing::make_signal_generator_module(
    signal_generator_module::args<double>);
template processing::signal_generator_module processing::make_signal_generator_module(
    signal_generator_module::args<float>);
