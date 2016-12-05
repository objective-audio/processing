//
//  yas_processing_stream.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time_range.h"

namespace yas {
namespace processing {
    class channel;

    class stream : public base {
       public:
        explicit stream(processing::time_range);
        stream(std::nullptr_t);

        processing::time_range time_range() const;

        void insert_channel(int64_t const);
        bool channel_exists(int64_t const);
        processing::channel const &channel(uint64_t const) const;
        processing::channel &channel(uint64_t const);
        std::size_t channel_count() const;

       private:
        class impl;
    };
}
}
