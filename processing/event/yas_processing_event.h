//
//  yas_processing_event.h
//

#pragma once

#include <processing/yas_processing_ptr.h>

#include <memory>

namespace yas::proc {
class time;

enum event_type {
    number,
    signal,
};

struct event {
    event(number_event_ptr const &);
    event(signal_event_ptr const &);

    virtual ~event() = default;

    virtual bool validate_time(time const &) const = 0;
    virtual event_ptr copy() const = 0;
    virtual bool is_equal(event_ptr const &) const;

    event_type type() const;

   protected:
    event();

   private:
    number_event_ptr _number = nullptr;
    signal_event_ptr _signal = nullptr;
};
}  // namespace yas::proc
