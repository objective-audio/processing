//
//  yas_processing_math2_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace math2 {
        enum class kind {
            plus,
            minus,
            multiply,
            divide,

            atan2,

            pow,
            hypot,
        };

        enum class output : connector_index_t {
            result,
        };

        enum class input_key : connector_index_t {
            left,
            right,
        };
    }

    template <typename T>
    module make_signal_module(math2::kind const);
}
}
