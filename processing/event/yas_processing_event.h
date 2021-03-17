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

struct event final {
    event(number_event_ptr const &);
    event(signal_event_ptr const &);

    bool validate_time(time const &) const;
    event copy() const;
    bool is_equal(event const &) const;
    std::type_info const &sample_type() const;

    event_type type() const;
    template <typename T>
    std::shared_ptr<T> const &get() const;

   protected:
    event();

   private:
    number_event_ptr _number = nullptr;
    signal_event_ptr _signal = nullptr;
};

std::type_info const &to_time_type(event_type const);
}  // namespace yas::proc
