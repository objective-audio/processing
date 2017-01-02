//
//  yas_processing_channel.h
//

#pragma once

#include "yas_base.h"
#include <map>

namespace yas {
namespace processing {
    class time_range;
    class buffer;

    class channel : public base {
        class impl;

       public:
        channel();
        channel(std::nullptr_t);

        std::multimap<time_range, buffer> const &buffers() const;
        std::multimap<time_range, buffer> &buffers();

        void insert_buffer(time_range, buffer);

        template <typename P>
        void erase_buffer_if(P predicate);
    };
}
}

#include "yas_processing_channel_private.h"
