//
//  yas_processing_math_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace math {
        enum class output_key : connector_index_t {
            out,
        };

        enum class input_key : connector_index_t {
            left_in,
            right_in,
        };

        template <typename T>
        module make_plus_signal_module();
        template <typename T>
        module make_minus_signal_module();
        template <typename T>
        module make_multiply_signal_module();
        template <typename T>
        module make_divide_signal_module();
    }
}
}
