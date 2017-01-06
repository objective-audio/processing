//
//  yas_processing_number_event.cpp
//

#include "yas_processing_number_event.h"

using namespace yas;

processing::number_event::number_event(std::nullptr_t) : event(nullptr) {
}

std::type_info const &processing::number_event::sample_type() const {
    return impl_ptr<impl>()->type();
}

std::size_t processing::number_event::sample_byte_count() const {
    return impl_ptr<impl>()->sample_byte_count();
}
