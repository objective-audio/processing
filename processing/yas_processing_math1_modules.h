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

            exp,
            exp2,
            expm1,
            log,
            log10,
            log1p,
            log2,

            sqrt,
            cbrt,
            abs,

            ceil,
            floor,
            trunc,
            round,
        };

        enum class input : connector_index_t {
            parameter,
        };

        enum class output : connector_index_t {
            result,
        };
    }

    template <typename T>
    module make_signal_module(math1::kind const);
    
    template <typename T>
    module make_number_module(math1::kind const);
}
}
