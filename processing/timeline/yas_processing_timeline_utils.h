//
//  yas_processing_timeline_utils.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>
#include <processing/yas_processing_time.h>
#include <processing/yas_processing_timeline_types.h>

namespace yas::proc {
timeline_event_type to_timeline_event_type(observing::map::event_type const &);

timeline_track_map_t copy_tracks(timeline_track_map_t const &);

std::optional<time::range> total_range(std::map<track_index_t, track_ptr> const &);
}  // namespace yas::proc
