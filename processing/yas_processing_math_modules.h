//
//  yas_processing_math_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace math {
        enum class operator_type {
            plus,
            minus,
            multiply,
            divide,
        };
        
        enum class output_key : connector_index_t {
            out,
        };

        enum class input_key : connector_index_t {
            left_in,
            right_in,
        };
        
        template <typename T>
        module make_signal_module(operator_type const);
    }
}
}
