//
//  yas_processing_routing_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace routing {
        enum class kind {
            move,
            copy,
        };

        enum class input : connector_index_t {
            value,
        };

        enum class output : connector_index_t {
            value,
        };
    }

    template <typename T>
    module make_signal_module(routing::kind const);

    template <typename T>
    module make_number_module(routing::kind const);
}
}
