//
//  yas_processing_channel.cpp
//

#include "yas_processing_channel.h"
#include "yas_processing_event.h"
#include "yas_processing_signal_event.h"

using namespace yas;

#pragma mark - processing::channel::impl

struct processing::channel::impl : base::impl {
    events_map_t _events;

    impl() {
    }

    impl(events_map_t &&events) : _events(std::move(events)) {
    }
};

#pragma mark - processing::channel

processing::channel::channel() : base(std::make_shared<impl>()) {
}

processing::channel::channel(events_map_t events) : base(std::make_shared<impl>(std::move(events))) {
}

processing::channel::channel(std::nullptr_t) : base(nullptr) {
}

processing::channel::events_map_t const &processing::channel::events() const {
    return this->impl_ptr<impl>()->_events;
}

processing::channel::events_map_t &processing::channel::events() {
    return this->impl_ptr<impl>()->_events;
}

void processing::channel::insert_event(time time, event event) {
    if (!event.validate_time(time)) {
        throw "invalid time for event.";
    }

    this->impl_ptr<impl>()->_events.emplace(std::move(time), std::move(event));
}

processing::channel::events_map_t processing::channel::copied_events(time::range const &copy_range) const {
    events_map_t result;

    for (auto const &event_pair : this->events()) {
        auto const &event_time = event_pair.first;
        auto const &event = event_pair.second;
        if (event_time.is_any_type()) {
            result.emplace(std::make_pair(event_time, event.copy()));
        } else if (event_time.is_frame_type()) {
            if (copy_range.is_contain(event_time.get<time::frame>())) {
                result.emplace(std::make_pair(event_time, event.copy()));
            }
        } else if (event_time.is_range_type()) {
            auto const &event_range = event_time.get<time::range>();
            if (auto const overlap_range_opt = copy_range.intersect(event_range)) {
                auto const &overlap_range = *overlap_range_opt;
                auto const signal = cast<signal_event>(event);
                auto copied_signal =
                    signal.copy_in_range(time::range{overlap_range.frame - event_range.frame, overlap_range.length});
                result.emplace(std::make_pair(time{overlap_range}, std::move(copied_signal)));
            }
        } else {
            throw "unreachable code.";
        }
    }

    return result;
}

void processing::channel::erase_events(time::range const &erase_range) {
    signal_event::pair_vector_t remained_signal;

    erase_if(this->events(), [erase_range, &remained_signal](auto const &event_pair) {
        time const &event_time = event_pair.first;
        if (event_time.is_any_type()) {
            return true;
        } else if (event_time.is_frame_type()) {
            if (erase_range.is_contain(event_time.get<time::frame>())) {
                return true;
            }
        } else if (event_time.is_range_type()) {
            auto const &event_range = event_time.get<time::range>();
            if (auto overlapped_range = erase_range.intersect(event_range)) {
                auto const signal = cast<signal_event>(event_pair.second);
                auto const range = time::range{overlapped_range->frame - event_range.frame, overlapped_range->length};
                signal_event::pair_vector_t erased_signals = signal.erased_in_range(range);
                for (auto const &erased_signal : erased_signals) {
                    auto const &erased_range = erased_signal.first;
                    remained_signal.emplace_back(
                        std::make_pair(time::range{erased_range.frame + event_range.frame, erased_range.length},
                                       erased_signal.second));
                }

                return true;
            }
        } else {
            throw "unreachable code.";
        }

        return false;
    });

    if (remained_signal.size() > 0) {
        for (auto const &signal_pair : remained_signal) {
            this->insert_event(time{signal_pair.first}, signal_pair.second);
        }
    }
}
