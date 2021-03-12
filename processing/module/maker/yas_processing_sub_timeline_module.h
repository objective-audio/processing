//
//  yas_processing_timeline_module.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_ptr.h>

namespace yas::proc {
module_ptr make_module(timeline_ptr const &, frame_index_t const offset = 0);
}  // namespace yas::proc
