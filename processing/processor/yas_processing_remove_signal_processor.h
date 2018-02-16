//
//  yas_processing_remove_signal_processor.h
//

#pragma once

#include "yas_processing_processor.h"
#include <unordered_set>

namespace yas::processing {
template <typename T>
processor_f make_remove_signal_processor(connector_index_set_t);
}
