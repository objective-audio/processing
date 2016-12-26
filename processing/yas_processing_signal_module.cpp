//
//  yas_processing_signal_module.cpp
//

#include "yas_processing_signal_module.h"

using namespace yas;

processing::signal_module::signal_module(std::shared_ptr<signal_module::impl_base> &&impl) : module(std::move(impl)) {
}

processing::signal_module::signal_module(std::nullptr_t) : module(nullptr) {
}

template <typename T>
processing::signal_module processing::make_signal_module(signal_module::args<T> args) {
    return signal_module{std::make_shared<signal_module::impl<T>>(std::move(args))};
}

template processing::signal_module processing::make_signal_module(signal_module::args<double>);
template processing::signal_module processing::make_signal_module(signal_module::args<float>);
