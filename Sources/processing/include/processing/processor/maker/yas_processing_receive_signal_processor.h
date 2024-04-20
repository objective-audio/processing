//
//  yas_processing_receive_signal_processor.h
//

#pragma once

#include <processing/common/yas_processing_common_types.h>
#include <processing/processor/yas_processing_processor.h>
#include <processing/sync_source/yas_processing_sync_source.h>
#include <processing/time/yas_processing_time.h>

#include <functional>

namespace yas::proc {
template <typename T>
using receive_signal_process_f = std::function<void(proc::time::range const &, sync_source const &,
                                                    channel_index_t const, connector_index_t const, T const *const)>;

template <typename T>
[[nodiscard]] processor_f make_receive_signal_processor(receive_signal_process_f<T>);
}  // namespace yas::proc
