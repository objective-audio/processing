//
//  yas_processing_timeline.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time.h"
#include "yas_processing_types.h"
#include <experimental/optional>

namespace yas {
namespace processing {
    class track;
    class stream;

    class timeline : public base {
        class impl;

       public:
        using track_map_t = std::map<track_index_t, track>;

        timeline();
        timeline(std::nullptr_t);

        track_map_t const &tracks() const;
        track_map_t &tracks();

        void insert_track(track_index_t const, track);
        track &add_track(track_index_t const);
        std::size_t track_count() const;
        bool has_track(track_index_t const) const;
        processing::track const &track(track_index_t const) const;
        processing::track &track(track_index_t const);

        void process(time::range const &, stream &);
    };
}
}
