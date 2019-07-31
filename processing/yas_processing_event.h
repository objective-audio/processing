//
//  yas_processing_event.h
//

#pragma once

#include <memory>

namespace yas::proc {
class time;

struct event {
    virtual ~event() = default;

    virtual bool validate_time(time const &) const = 0;
    virtual std::shared_ptr<event> copy() const = 0;
    virtual bool is_equal(std::shared_ptr<event> const &) const;
};
}  // namespace yas::proc
