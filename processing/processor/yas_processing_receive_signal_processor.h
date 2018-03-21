//
//  yas_processing_receive_signal_processor.h
//

#pragma once

#include "yas_processing_types.h"
#include "yas_processing_time.h"
#include "yas_processing_processor.h"
#include "yas_processing_sync_source.h"
#include <functional>

namespace yas::proc {
template <typename T>
using receive_signal_process_f = std::function<void(proc::time::range const &, sync_source const &,
                                                    channel_index_t const, connector_index_t const, T const *const)>;

template <typename T>
processor_f make_receive_signal_processor(receive_signal_process_f<T>);
}
