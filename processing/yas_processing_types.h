//
//  yas_processing_types.h
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_set>

namespace yas::proc {
class track;
class timeline;
class module;
class number_event;
class signal_event;

using track_ptr = std::shared_ptr<track>;
using timeline_ptr = std::shared_ptr<timeline>;
using module_ptr = std::shared_ptr<module>;
using number_event_ptr = std::shared_ptr<number_event>;
using signal_event_ptr = std::shared_ptr<signal_event>;

using channel_index_t = int64_t;
using track_index_t = int64_t;
using frame_index_t = int64_t;
using length_t = uint64_t;
using sample_rate_t = uint32_t;

using connector_index_t = uint32_t;
using connector_index_set_t = std::unordered_set<connector_index_t>;

enum class continuation {
    abort,
    keep,
};
}  // namespace yas::proc
