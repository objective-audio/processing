//
//  yas_processing_cast_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace cast {
        enum class output_key : connector_index_t {
            out,
        };

        enum class input_key : connector_index_t {
            in,
        };

        template <typename In, typename Out>
        module make_signal_module();

        template <typename In, typename Out>
        module make_number_module();
    }
}
}

#include "yas_processing_cast_module_private.h"
