//
//  yas_processing_routing.h
//

#pragma once

#include "yas_base.h"
#include <experimental/optional>

namespace yas {
namespace processing {
    class track;

    class routing : public base {
        class impl;

       public:
        routing();
        routing(std::nullptr_t);

        std::map<int64_t, track> const &tracks() const;
        std::map<int64_t, track> &tracks();

        void insert_track(int64_t const, track);
        std::size_t track_count() const;
        bool has_track(int64_t const) const;
        std::experimental::optional<track> track(int64_t const) const;
    };
}
}
