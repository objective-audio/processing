//
//  yas_processing_timeline_module.h
//

#pragma once

#include "yas_processing_ptr.h"
#include "yas_processing_types.h"

namespace yas::proc {
module_ptr make_module(timeline_ptr const &, frame_index_t const offset = 0);
}  // namespace yas::proc
