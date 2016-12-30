//
//  yas_processing_timeline.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_types.h"
#include <experimental/optional>

namespace yas {
namespace processing {
    class track;
    class stream;

    class timeline : public base {
        class impl;

       public:
        timeline();
        timeline(std::nullptr_t);

        std::map<track_index_t, track> const &tracks() const;
        std::map<track_index_t, track> &tracks();

        void insert_track(track_index_t const, track);
        std::size_t track_count() const;
        bool has_track(track_index_t const) const;
        std::experimental::optional<track> track(track_index_t const) const;
        
        void process(stream &);
    };
}
}
