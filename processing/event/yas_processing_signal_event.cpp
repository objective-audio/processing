//
//  yas_processing_signal_event.cpp
//

#include "yas_processing_signal_event.h"

using namespace yas;

proc::signal_event::signal_event(std::nullptr_t) : event(nullptr) {
}

std::type_info const &proc::signal_event::sample_type() const {
    return this->impl_ptr<impl>()->type();
}

std::size_t proc::signal_event::sample_byte_count() const {
    return this->impl_ptr<impl>()->sample_byte_count();
}

std::size_t proc::signal_event::size() const {
    return this->impl_ptr<impl>()->size();
}

std::size_t proc::signal_event::byte_size() const {
    return this->impl_ptr<impl>()->byte_size();
}

void proc::signal_event::resize(std::size_t const size) {
    return this->impl_ptr<impl>()->resize(size);
}

void proc::signal_event::reserve(std::size_t const size) {
    return this->impl_ptr<impl>()->reserve(size);
}

proc::signal_event proc::signal_event::copy_in_range(time::range const &range) const {
    return this->impl_ptr<impl>()->copy_in_range(range);
}

std::vector<std::pair<proc::time::range, proc::signal_event>> proc::signal_event::cropped(
    time::range const &range) const {
    return this->impl_ptr<impl>()->cropped(range);
}

proc::signal_event::pair_t proc::signal_event::combined(time::range const &insert_range, pair_vector_t event_pairs) {
    return this->impl_ptr<impl>()->combined(insert_range, event_pairs);
}
