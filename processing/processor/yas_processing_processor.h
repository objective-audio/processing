//
//  yas_processing_processor.h
//

#pragma once

#include <processing/yas_processing_connector.h>
#include <processing/yas_processing_time.h>

#include <functional>

namespace yas::proc {
class stream;

using processor_f =
    std::function<void(time::range const &, connector_map_t const &inputs, connector_map_t const &outputs, stream &)>;
}  // namespace yas::proc
