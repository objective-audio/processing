//
//  yas_processing_event.cpp
//

#include "yas_processing_event.h"

#include "yas_processing_time.h"

using namespace yas;
using namespace yas::proc;

event::event() = default;

event::event(number_event_ptr const &number) : _number(number) {
}

event::event(signal_event_ptr const &signal) : _signal(signal) {
}

bool event::is_equal(event_ptr const &rhs) const {
    return reinterpret_cast<uintptr_t>(this) == reinterpret_cast<uintptr_t>(rhs.get());
}

event_type event::type() const {
    if (this->_number) {
        return event_type::number;
    } else if (this->_signal) {
        return event_type::signal;
    } else {
        throw std::runtime_error("event not found.");
    }
}
