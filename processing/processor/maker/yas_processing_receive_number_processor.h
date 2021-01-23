//
//  yas_processing_receive_number_processor.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_processor.h>
#include <processing/yas_processing_time.h>

#include <functional>

namespace yas::proc {
template <typename T>
using receive_number_process_f =
    std::function<void(proc::time::frame::type const &, channel_index_t const, connector_index_t const, T const &)>;

template <typename T>
processor_f make_receive_number_processor(receive_number_process_f<T>);
}  // namespace yas::proc
