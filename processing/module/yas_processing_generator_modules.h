//
//  yas_processing_generator_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace generator {
        enum class kind {
            second,
        };

        enum class output : connector_index_t {
            value,
        };
    }

    template <typename T>
    module make_signal_module(generator::kind const, frame_index_t const offset);
}

void connect(processing::module &, processing::generator::output const &, processing::channel_index_t const &);

std::string to_string(processing::generator::output const &);
}
