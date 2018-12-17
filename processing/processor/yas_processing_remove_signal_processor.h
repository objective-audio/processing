//
//  yas_processing_remove_signal_processor.h
//

#pragma once

#include <unordered_set>
#include "yas_processing_processor.h"

namespace yas::proc {
template <typename T>
processor_f make_remove_signal_processor(connector_index_set_t);
}
