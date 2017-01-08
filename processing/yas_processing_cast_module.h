//
//  yas_processing_cast_module.h
//

#pragma once

#include "yas_processing_signal_event.h"

namespace yas {
namespace processing {
    class module;

    namespace cast {
        static auto constexpr out_connector_key = "out";
        static auto constexpr in_connector_key = "in";

        template <typename In, typename Out>
        module make_signal_module();
    }
}
}

#include "yas_processing_cast_module_private.h"
