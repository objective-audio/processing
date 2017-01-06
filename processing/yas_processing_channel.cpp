//
//  yas_processing_channel.cpp
//

#include "yas_processing_channel.h"
#include "yas_processing_event.h"
#include "yas_processing_signal_event.h"

using namespace yas;

#pragma mark - processing::channel::impl

struct processing::channel::impl : base::impl {
    std::multimap<time, event> _events;
};

#pragma mark - processing::channel

processing::channel::channel() : base(std::make_shared<impl>()) {
}

processing::channel::channel(std::nullptr_t) : base(nullptr) {
}

std::multimap<processing::time, processing::event> const &processing::channel::events() const {
    return impl_ptr<impl>()->_events;
}

std::multimap<processing::time, processing::event> &processing::channel::events() {
    return impl_ptr<impl>()->_events;
}

void processing::channel::insert_event(time time, event event) {
    if (!event.validate_time(time)) {
        throw "invalid time for event.";
    }

    impl_ptr<impl>()->_events.emplace(std::move(time), std::move(event));
}
