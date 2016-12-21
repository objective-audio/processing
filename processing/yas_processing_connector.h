//
//  yas_processing_connector.h
//

#pragma once

#include <string>
#include <experimental/optional>

namespace yas {
namespace processing {
    struct connector {
        using channel_opt_t = std::experimental::optional<int64_t>;
        
        channel_opt_t channel;
    };
}
}
