//
//  yas_processing_types.h
//

#pragma once

#include <cstdint>
#include <unordered_set>

namespace yas {
namespace processing {
    static auto constexpr reserved_signal_size = 1024;

    using channel_index_t = int64_t;
    using track_index_t = int64_t;
    using frame_index_t = int64_t;
    using length_t = uint32_t;

    static length_t constexpr zero_length = 0;

    using connector_index_t = uint32_t;
    using connector_index_set_t = std::unordered_set<connector_index_t>;
}
}
