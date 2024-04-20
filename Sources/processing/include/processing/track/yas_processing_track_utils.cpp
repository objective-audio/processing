//
//  yas_processing_track_utils.cpp
//

#include "yas_processing_track_utils.h"

#include <processing/module/yas_processing_module.h>
#include <processing/module_set/yas_processing_module_set.h>

using namespace yas;
using namespace yas::proc;

track_event_type proc::to_track_event_type(observing::map::event_type const &map_type) {
    switch (map_type) {
        case observing::map::event_type::any:
            return track_event_type::any;
        case observing::map::event_type::inserted:
            return track_event_type::inserted;
        case observing::map::event_type::replaced:
            return track_event_type::replaced;
        case observing::map::event_type::erased:
            return track_event_type::erased;
    }
}

proc::track_module_set_map_t proc::copy_module_sets(track_module_set_map_t const &src_modules) {
    track_module_set_map_t result;
    for (auto const &pair : src_modules) {
        result.emplace(pair.first, pair.second->copy());
    }
    return result;
}
