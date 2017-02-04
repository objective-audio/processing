//
//  yas_processing_number_to_signal_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace number_to_signal {
        enum class output : connector_index_t {
            signal,
        };

        enum class input_key : connector_index_t {
            number,
        };
    }

    template <typename T>
    module make_number_to_signal_module();
}
}
