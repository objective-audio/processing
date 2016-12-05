//
//  yas_processing_time_range.h
//

#pragma once

#include <memory>

namespace yas {
namespace processing {
    struct time_range {
        int64_t const start_frame;
        uint32_t const length;

        bool operator==(time_range const &) const;
        bool operator!=(time_range const &) const;
        bool operator<(time_range const &) const;

        int64_t next_frame() const;
    };
}
}
