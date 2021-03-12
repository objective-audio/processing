//
//  yas_processing_track_utils.h
//

#pragma once

#include <processing/yas_processing_track_types.h>

namespace yas::proc {
track_event_type to_track_event_type(observing::map::event_type const &);

track_module_set_map_t copy_module_sets(track_module_set_map_t const &);
}  // namespace yas::proc
