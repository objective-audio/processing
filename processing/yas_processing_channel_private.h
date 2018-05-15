//
//  yas_processing_channel_private.h
//

#pragma once

#include "yas_processing_event.h"
#include "yas_stl_utils.h"

namespace yas {
template <typename P> void proc::channel::erase_event_if(P predicate) {
  erase_if(this->events(), predicate);
}

template <typename T, typename Event> void proc::channel::erase_event() {
  erase_event_if<T, Event>([](auto const &) { return true; });
}

template <typename T, typename Event, typename P>
void proc::channel::erase_event(P predicate) {
  erase_if(
      this->events(), [predicate = std::move(predicate)](auto const &pair) {
        time const &time = pair.first;
        if (time.type() == typeid(typename Event::time_type)) {
          if (auto const casted_event = yas::cast<Event>(pair.second)) {
            if (casted_event.sample_type() == typeid(T) &&
                predicate(std::make_pair(time.get<typename Event::time_type>(),
                                         casted_event))) {
              return true;
            }
          }
        }
        return false;
      });
}

template <typename T, typename Event>
std::multimap<typename Event::time_type::type, Event>
proc::channel::filtered_events() const {
  return filtered_events<T, Event>([](auto const &) { return true; });
}

template <typename T, typename Event, typename P>
std::multimap<typename Event::time_type::type, Event>
proc::channel::filtered_events(P predicate) const {
  std::multimap<typename Event::time_type::type, Event> filtered;

  for (auto const &event_pair : events()) {
    if (event_pair.first.type() == typeid(typename Event::time_type)) {
      if (auto const casted_event = yas::cast<Event>(event_pair.second)) {
        auto pair = std::make_pair(
            event_pair.first.get<typename Event::time_type>(), casted_event);
        if (casted_event.sample_type() == typeid(T) && predicate(pair)) {
          filtered.insert(filtered.end(), std::move(pair));
        }
      }
    }
  }

  return filtered;
}
} // namespace yas
