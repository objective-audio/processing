//
//  yas_processing_number_event_private.h
//

#pragma once

#include "yas_processing_time.h"

namespace yas {
struct proc::number_event::impl {
    virtual std::type_info const &type() const = 0;
    virtual std::size_t sample_byte_count() const = 0;
    virtual std::shared_ptr<number_event> copy() = 0;
    virtual bool is_equal(std::shared_ptr<number_event::impl> const &) const = 0;
};
}  // namespace yas
