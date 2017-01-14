//
//  yas_processing_channel_private.h
//

#pragma once

#include "yas_processing_signal_event.h"
#include "yas_processing_number_event.h"
#include "yas_stl_utils.h"

namespace yas {
template <typename P>
void processing::channel::erase_event_if(P predicate) {
    erase_if(this->events(), predicate);
}

template <typename T, typename Event>
void processing::channel::erase_event() {
    erase_event_if<T, Event>([](auto const &) { return true; });
}

template <typename T, typename Event, typename P>
void processing::channel::erase_event_if(P predicate) {
    erase_if(this->events(), [predicate = std::move(predicate)](auto const &pair) {
        if (pair.first.type() == typeid(typename Event::time_type)) {
            if (auto const casted_event = yas::cast<Event>(pair.second)) {
                if (casted_event.sample_type() == typeid(T) && predicate(pair)) {
                    return true;
                }
            }
        }
        return false;
    });
}

template <typename T, typename Event>
std::multimap<typename Event::time_type::type, Event> processing::channel::filtered_events() const {
    return filtered_events<T, Event>([](auto const &) { return true; });
}

template <typename T, typename Event, typename P>
std::multimap<typename Event::time_type::type, Event> processing::channel::filtered_events(P predicate) const {
    std::multimap<typename Event::time_type::type, Event> filtered;

    for (auto const &pair : events()) {
        if (pair.first.type() == typeid(typename Event::time_type)) {
            if (auto const casted_event = yas::cast<Event>(pair.second)) {
                if (casted_event.sample_type() == typeid(T) && predicate(pair)) {
                    filtered.insert(filtered.end(), {pair.first.get<typename Event::time_type>(), casted_event});
                }
            }
        }
    }

    return filtered;
}
}
