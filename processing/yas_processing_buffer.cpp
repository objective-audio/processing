//
//  yas_processing_buffer.cpp
//

#include "yas_processing_buffer.h"

using namespace yas;

processing::buffer::buffer(std::nullptr_t) : event(nullptr) {
}

std::type_info const &processing::buffer::sample_type() const {
    return impl_ptr<impl>()->type();
}

std::size_t processing::buffer::sample_byte_count() const {
    return impl_ptr<impl>()->sample_byte_count();
}

std::size_t processing::buffer::size() const {
    return impl_ptr<impl>()->size();
}

void processing::buffer::resize(std::size_t const size) {
    return impl_ptr<impl>()->resize(size);
}
