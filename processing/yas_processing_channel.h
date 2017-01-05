//
//  yas_processing_channel.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time.h"
#include <map>

namespace yas {
namespace processing {
    class buffer;

    class channel : public base {
        class impl;

       public:
        channel();
        channel(std::nullptr_t);

        std::multimap<time, buffer> const &buffers() const;
        std::multimap<time, buffer> &buffers();

        void insert_buffer(time, buffer);

        template <typename P>
        void erase_buffer_if(P predicate);
    };
}
}

#include "yas_processing_channel_private.h"
