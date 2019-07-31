//
//  yas_processing_event.cpp
//

#include "yas_processing_event.h"
#include "yas_processing_time.h"

using namespace yas;

bool proc::event::is_equal(std::shared_ptr<event> const &rhs) const {
    return reinterpret_cast<uintptr_t>(this) == reinterpret_cast<uintptr_t>(rhs.get());
}
