//
//  yas_processing_channel_private.h
//

#pragma once

#include <cpp_utils/yas_stl_utils.h>
#include "yas_processing_event.h"

namespace yas {
template <typename P>
void proc::channel::erase_event_if(P predicate) {
    erase_if(this->events(), predicate);
}

template <typename SampleType, typename Event>
void proc::channel::erase_event() {
    erase_event_if<SampleType, Event>([](auto const &) { return true; });
}

template <typename SampleType, typename Event, typename P>
void proc::channel::erase_event(P predicate) {
    erase_if(this->events(), [predicate = std::move(predicate)](auto const &pair) {
        time const &time = pair.first;
        if (time.type() == typeid(typename Event::time_type)) {
            if (auto const casted_event = std::dynamic_pointer_cast<Event>(pair.second)) {
                if (casted_event->sample_type() == typeid(SampleType) &&
                    predicate(std::make_pair(time.get<typename Event::time_type>(), casted_event))) {
                    return true;
                }
            }
        }
        return false;
    });
}

template <typename Event>
std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> proc::channel::filtered_events() const {
    return filtered_events<Event>([](auto const &) { return true; });
}

template <typename Event, typename P>
std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> proc::channel::filtered_events(
    P predicate) const {
    std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> filtered;

    for (auto const &event_pair : events()) {
        if (event_pair.first.type() == typeid(typename Event::time_type)) {
            if (auto const casted_event = std::dynamic_pointer_cast<Event>(event_pair.second)) {
                auto pair = std::make_pair(event_pair.first.get<typename Event::time_type>(), casted_event);
                if (predicate(pair)) {
                    filtered.insert(filtered.end(), std::move(pair));
                }
            }
        }
    }

    return filtered;
}

template <typename SampleType, typename Event>
std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> proc::channel::filtered_events() const {
    return filtered_events<SampleType, Event>([](auto const &) { return true; });
}

template <typename SampleType, typename Event, typename P>
std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> proc::channel::filtered_events(
    P predicate) const {
    std::multimap<typename Event::time_type::type, std::shared_ptr<Event>> filtered;

    for (auto const &event_pair : events()) {
        if (event_pair.first.type() == typeid(typename Event::time_type)) {
            if (auto const casted_event = std::dynamic_pointer_cast<Event>(event_pair.second)) {
                auto pair = std::make_pair(event_pair.first.get<typename Event::time_type>(), casted_event);
                if (casted_event->sample_type() == typeid(SampleType) && predicate(pair)) {
                    filtered.insert(filtered.end(), std::move(pair));
                }
            }
        }
    }

    return filtered;
}
}  // namespace yas
