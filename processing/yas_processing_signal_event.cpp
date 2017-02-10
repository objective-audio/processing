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

void processing::signal_event::reserve(std::size_t const size) {
    return this->impl_ptr<impl>()->reserve(size);
}

processing::signal_event processing::signal_event::copy_in_range(time::range const &range) const {
    return this->impl_ptr<impl>()->copy_in_range(range);
}

std::vector<std::pair<processing::time::range, processing::signal_event>> processing::signal_event::erased_in_range(
    time::range const &range) {
    return this->impl_ptr<impl>()->erased_in_range(range);
}
