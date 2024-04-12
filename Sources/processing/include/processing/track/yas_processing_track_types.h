//
//  yas_processing_track_types.h
//

#pragma once

#include <processing/common/yas_processing_ptr.h>
#include <processing/module_set/yas_processing_module_set_types.h>
#include <processing/time/yas_processing_time.h>

#include <observing/yas_observing_umbrella.hpp>

namespace yas::proc {
using track_module_set_map_t = std::map<time::range, module_set_ptr>;

enum class track_event_type {
    any,
    replaced,
    inserted,
    erased,

    relayed,
};

struct track_event {
    track_event_type type;
    track_module_set_map_t const &module_sets;
    module_set_ptr const *inserted = nullptr;
    module_set_ptr const *erased = nullptr;
    module_set_ptr const *relayed = nullptr;
    std::optional<time::range> range = std::nullopt;
    module_set_event const *module_set_event = nullptr;
};
}  // namespace yas::proc
