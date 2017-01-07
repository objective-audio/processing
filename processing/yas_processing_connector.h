//
//  yas_processing_connector.h
//

#pragma once

#include "yas_processing_types.h"
#include <string>
#include <experimental/optional>
#include <map>

namespace yas {
namespace processing {
    struct connector {
        channel_index_t channel_index;
    };

    using connector_map_t = std::map<std::string, connector>;
}
}
