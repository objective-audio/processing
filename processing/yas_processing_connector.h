//
//  yas_processing_connector.h
//

#pragma once

#include <processing/yas_processing_types.h>

#include <map>
#include <optional>
#include <string>

namespace yas::proc {
struct connector {
    channel_index_t channel_index;
};

using connector_map_t = std::map<connector_index_t, connector>;

template <typename T>
connector_index_t to_connector_index(T const &);
}  // namespace yas::proc

#include <processing/yas_processing_connector_private.h>
