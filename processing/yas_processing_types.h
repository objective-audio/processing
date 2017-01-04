//
//  yas_processing_types.h
//

#pragma once

#include <cstdint>

namespace yas {
namespace processing {
    static auto constexpr reserved_buffer_size = 1024;

    using channel_index_t = int64_t;
    using track_index_t = int64_t;
    using frame_index_t = int64_t;
    using length_t = uint32_t;
}
}
