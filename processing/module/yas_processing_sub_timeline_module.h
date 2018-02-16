//
//  yas_processing_timeline_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas::processing {
class module;
class timeline;

module make_module(timeline, frame_index_t const offset = 0);
}
