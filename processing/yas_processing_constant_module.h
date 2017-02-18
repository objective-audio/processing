//
//  yas_processing_constant_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace constant {
        enum class output : connector_index_t {
            value,
        };
    }

    template <typename T>
    module make_signal_module(T);

    template <typename T>
    module make_number_module(T);
}
}
