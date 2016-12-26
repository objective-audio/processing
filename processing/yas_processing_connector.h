//
//  yas_processing_connector.h
//

#pragma once

#include <string>
#include <experimental/optional>

namespace yas {
namespace processing {
    struct connector {
        using channel_index_t = int64_t;

        channel_index_t channel_index;
    };
}
}
