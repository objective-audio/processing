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
        };

        enum class output_key : connector_index_t {
            out,
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
