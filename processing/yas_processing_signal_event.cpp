//
//  yas_processing_signal_event.cpp
//

#include "yas_processing_signal_event.h"

using namespace yas;

processing::signal_event::signal_event(std::nullptr_t) : event(nullptr) {
}

std::type_info const &processing::signal_event::sample_type() const {
    return this->impl_ptr<impl>()->type();
}

std::size_t processing::signal_event::sample_byte_count() const {
    return this->impl_ptr<impl>()->sample_byte_count();
}

std::size_t processing::signal_event::size() const {
    return this->impl_ptr<impl>()->size();
}

void processing::signal_event::resize(std::size_t const size) {
    return this->impl_ptr<impl>()->resize(size);
}
