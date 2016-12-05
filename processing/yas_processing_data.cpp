//
//  yas_processing_data.cpp
//

#include "yas_processing_data.h"

using namespace yas;

std::type_info const &processing::data::sample_type() const {
    return impl_ptr<impl_base>()->type();
}

std::size_t processing::data::sample_byte_count() const {
    return impl_ptr<impl_base>()->sample_byte_count();
}
