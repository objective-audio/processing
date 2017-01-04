//
//  yas_processing_math_modules.h
//

#pragma once

namespace yas {
namespace processing {
    class module;

    namespace plus {
        static auto constexpr out_connector_key = "out";
        static auto constexpr left_in_connector_key = "left_in";
        static auto constexpr right_in_connector_key = "right_in";

        template <typename T>
        module make_signal_module();
    }
}
}
