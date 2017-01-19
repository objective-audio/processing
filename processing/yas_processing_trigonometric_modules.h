//
//  yas_processing_trigonometric_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace trigonometric {
        enum class kind {
            sin,
        };

        enum class input_key : connector_index_t {
            phase,
        };

        enum class output_key : connector_index_t {
            out,
        };
    }

    template <typename T>
    module make_signal_module(trigonometric::kind const);
}
}
