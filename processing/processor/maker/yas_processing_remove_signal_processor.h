//
//  yas_processing_remove_signal_processor.h
//

#pragma once

#include <processing/yas_processing_processor.h>

#include <unordered_set>

namespace yas::proc {
template <typename T>
[[nodiscard]] processor_f make_remove_signal_processor(connector_index_set_t);
}
