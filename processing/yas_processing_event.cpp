//
//  yas_processing_event.cpp
//

#include "yas_processing_event.h"
#include "yas_processing_time.h"

using namespace yas;

proc::event::event(std::shared_ptr<impl> const &impl) : base(impl) {
}

proc::event::event(std::shared_ptr<impl> &&impl) : base(std::move(impl)) {
}

proc::event::event(std::nullptr_t) : base(nullptr) {
}

bool proc::event::validate_time(time const &time) const {
    return this->impl_ptr<impl>()->validate_time(time);
}

proc::event proc::event::copy() const {
    return this->impl_ptr<impl>()->copy();
}
