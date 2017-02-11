//
//  yas_processing_stream.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_types.h"
#include "yas_processing_time.h"
#include "yas_processing_channel.h"

namespace yas {
namespace processing {
    class sync_source;

    class stream : public base {
       public:
        explicit stream(sync_source);
        stream(std::nullptr_t);

        sync_source const &sync_source() const;

        processing::channel &add_channel(channel_index_t const);
        processing::channel &add_channel(channel_index_t const, channel::events_map_t);
        void remove_channel(channel_index_t const);
        bool has_channel(channel_index_t const);
        processing::channel const &channel(channel_index_t const) const;
        processing::channel &channel(channel_index_t const);
        std::size_t channel_count() const;

       private:
        class impl;
    };
}
}
