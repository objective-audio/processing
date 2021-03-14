//
//  yas_processing_timeline_types.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_ptr.h>
#include <processing/yas_processing_track_types.h>

#include <map>

namespace yas::proc {
using timeline_track_map_t = std::map<track_index_t, track_ptr>;

enum class timeline_event_type {
    any,
    replaced,
    inserted,
    erased,

    relayed,
};

struct timeline_event {
    timeline_event_type type;
    timeline_track_map_t const &tracks;
    track_ptr const *track = nullptr;
    std::optional<track_index_t> index = std::nullopt;
    track_event const *track_event = nullptr;
};
}  // namespace yas::proc
