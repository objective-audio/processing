//
//  yas_processing_math1_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace math1 {
        enum class kind {
            sin,
            cos,
            tan,
            asin,
            acos,
            atan,

            sinh,
            cosh,
            tanh,
            asinh,
            acosh,
            atanh,

            sqrt,
            cbrt,
            abs,
        };

        enum class input_key : connector_index_t {
            in,
        };

        enum class output_key : connector_index_t {
            out,
        };
    }

    template <typename T>
    module make_signal_module(math1::kind const);
}
}
