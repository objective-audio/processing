//
//  yas_processing_channel.h
//

#pragma once

#include <map>
#include "yas_processing_time.h"

namespace yas::proc {
class event;
class signal_event;

struct channel {
    using events_map_t = std::multimap<time, event_ptr>;

    channel();
    explicit channel(events_map_t const &);
    explicit channel(events_map_t &&);

    channel(channel &&) = default;
    channel &operator=(channel &&) = default;

    events_map_t const &events() const;
    events_map_t &events();

    template <typename Event>
    std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> filtered_events() const;
    template <typename Event, typename P>
    std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> filtered_events(P predicate) const;
    template <typename SampleType, typename Event>
    std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> filtered_events() const;
    template <typename SampleType, typename Event, typename P>
    std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> filtered_events(P predicate) const;
    events_map_t copied_events(time::range const &, frame_index_t const offset) const;

    void insert_event(time, event_ptr);
    void insert_events(events_map_t);

    std::pair<time::range, signal_event_ptr> combine_signal_event(time::range const &, signal_event_ptr const &);

    template <typename P>
    void erase_event_if(P predicate);
    template <typename SampleType, typename Event>
    void erase_event();
    template <typename SampleType, typename Event, typename P>
    void erase_event(P predicate);
    void erase_events(time::range const &);

   private:
    events_map_t _events;

    channel(channel const &) = delete;
    channel &operator=(channel const &) = delete;
};
}  // namespace yas::proc

#include "yas_processing_channel_private.h"
