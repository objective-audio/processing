//
//  yas_processing_stream.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time_range.h"
#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class channel;

    class stream : public base {
       public:
        explicit stream(processing::time_range);
        stream(std::nullptr_t);

        processing::time_range const &time_range() const;

        void insert_channel(channel_index_t const);
        bool has_channel(channel_index_t const);
        processing::channel const &channel(channel_index_t const) const;
        processing::channel &channel(channel_index_t const);
        std::size_t channel_count() const;

       private:
        class impl;
    };
}
}
