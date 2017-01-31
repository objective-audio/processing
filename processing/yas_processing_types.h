//
//  yas_processing_types.h
//

#pragma once

#include <cstdint>
#include <unordered_set>

namespace yas {
namespace processing {
    using channel_index_t = int64_t;
    using track_index_t = int64_t;
    using frame_index_t = int64_t;
    using length_t = uint32_t;
    using sample_rate_t = uint32_t;

    using connector_index_t = uint32_t;
    using connector_index_set_t = std::unordered_set<connector_index_t>;
    
    struct boolean {
        bool value = false;
        
        bool operator==(boolean const &rhs) const {
            return value == rhs.value;
        }
        
        bool operator!=(boolean const &rhs) const {
            return value != rhs.value;
        }
    };
}
}
