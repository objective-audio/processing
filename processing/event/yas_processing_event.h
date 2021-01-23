//
//  yas_processing_event.h
//

#pragma once

#include <processing/yas_processing_ptr.h>

#include <memory>

namespace yas::proc {
class time;

struct event {
    virtual ~event() = default;

    virtual bool validate_time(time const &) const = 0;
    virtual event_ptr copy() const = 0;
    virtual bool is_equal(event_ptr const &) const;
};
}  // namespace yas::proc
