//
//  yas_processing_constant_module.h
//

#pragma once

namespace yas {
namespace processing {
    class module;
    class time_range;

    namespace constant {
        static auto constexpr out_connector_key = "out";

        template <typename T>
        module make_signal_module(T);
    }
}
}
