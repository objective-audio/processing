//
//  yas_processing_module_set_utils.h
//

#pragma once

#include <processing/yas_processing_module_set_types.h>

namespace yas::proc {
[[nodiscard]] module_set_event_type to_module_set_event_type(observing::vector::event_type const &);
}
