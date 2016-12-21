//
//  yas_processing_time_range.h
//

#pragma once

#include <memory>
#include <experimental/optional>

namespace yas {
namespace processing {
    struct time_range {
        int64_t start_frame = 0;
        uint32_t length = 0;

        bool operator==(time_range const &) const;
        bool operator!=(time_range const &) const;
        bool operator<(time_range const &) const;

        int64_t next_frame() const;
        
        bool is_contain(time_range const &) const;
        bool is_overlap(time_range const &) const;
        bool can_combine(time_range const &) const;
        std::experimental::optional<time_range> intersect(time_range const &) const;
        std::experimental::optional<time_range> combine(time_range const &) const;
    };
}
}
