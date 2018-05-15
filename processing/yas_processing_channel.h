//
//  yas_processing_channel.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time.h"

namespace yas::proc {
class event;
class signal_event;

class channel : public base {
  class impl;

public:
  using events_map_t = std::multimap<time, event>;

  channel();
  explicit channel(events_map_t);
  channel(std::nullptr_t);

  events_map_t const &events() const;
  events_map_t &events();

  template <typename T, typename Event>
  std::multimap<typename Event::time_type::type, Event> filtered_events() const;
  template <typename T, typename Event, typename P>
  std::multimap<typename Event::time_type::type, Event>
  filtered_events(P predicate) const;
  events_map_t copied_events(time::range const &,
                             frame_index_t const offset) const;

  void insert_event(time, event);
  void insert_events(events_map_t);

  std::pair<time::range, signal_event> combine_signal_event(time::range const &,
                                                            signal_event);

  template <typename P> void erase_event_if(P predicate);
  template <typename T, typename Event> void erase_event();
  template <typename T, typename Event, typename P>
  void erase_event(P predicate);
  void erase_events(time::range const &);
};
} // namespace yas::proc

#include "yas_processing_channel_private.h"
