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

        void insert_event(time, event);

        template <typename P>
        void erase_event_if(P predicate);
    };
}
}

#include "yas_processing_channel_private.h"
