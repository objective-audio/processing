//
//  yas_processing_send_number_processor.h
//

#pragma once

#include <functional>
#include "yas_processing_number_event.h"
#include "yas_processing_processor.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_time.h"
#include "yas_processing_types.h"

namespace yas::proc {
template <typename T>
using send_number_process_f = std::function<number_event::value_map_t<T>(
    proc::time::range const &, sync_source const &, channel_index_t const, connector_index_t const)>;

template <typename T>
processor_f make_send_number_processor(send_number_process_f<T>);
}  // namespace yas::proc
