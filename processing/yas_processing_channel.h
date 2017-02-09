//
//  yas_processing_channel.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time.h"

namespace yas {
namespace processing {
    class event;

    class channel : public base {
        class impl;

       public:
        channel();
        channel(std::nullptr_t);

        std::multimap<time, event> const &events() const;
        std::multimap<time, event> &events();

        template <typename T, typename Event>
        std::multimap<typename Event::time_type::type, Event> filtered_events() const;
        template <typename T, typename Event, typename P>
        std::multimap<typename Event::time_type::type, Event> filtered_events(P predicate) const;

        void insert_event(time, event);

        template <typename P>
        void erase_event_if(P predicate);
        template <typename T, typename Event>
        void erase_event();
        template <typename T, typename Event, typename P>
        void erase_event(P predicate);
    };
}
}

#include "yas_processing_channel_private.h"
