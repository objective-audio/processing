//
//  yas_processing_module_utils.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_time.h>

namespace yas::proc {
// offsetはmoduleの置いてある位置。module内のframeを返す
frame_index_t module_frame(frame_index_t const &time_frame, frame_index_t const offset);
}  // namespace yas::proc
