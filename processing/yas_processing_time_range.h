//
//  yas_processing_range.h
//

#pragma once

#include "yas_processing_types.h"
#include "yas_base.h"
#include <memory>
#include <experimental/optional>

namespace yas {
namespace processing {
    struct time : public base {
       public:
        struct range {
            frame_index_t frame = 0;
            length_t length = 0;

            bool operator==(range const &) const;
            bool operator!=(range const &) const;
            bool operator<(range const &) const;

            frame_index_t next_frame() const;

            bool is_contain(range const &) const;
            bool is_overlap(range const &) const;
            bool can_combine(range const &) const;
            std::experimental::optional<range> intersect(range const &) const;
            std::experimental::optional<range> combine(range const &) const;
        };
    };
}
}
