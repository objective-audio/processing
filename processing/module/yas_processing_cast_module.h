//
//  yas_processing_cast_module.h
//

#pragma once

#include "yas_processing_types.h"
#include <string>

namespace yas {
namespace processing {
    class module;

    namespace cast {
        enum class output : connector_index_t {
            value,
        };

        enum class input : connector_index_t {
            value,
        };

        template <typename In, typename Out>
        module make_signal_module();

        template <typename In, typename Out>
        module make_number_module();
    }
}

void connect(processing::module &, processing::cast::input const &, processing::channel_index_t const &);
void connect(processing::module &, processing::cast::output const &, processing::channel_index_t const &);

std::string to_string(processing::cast::input const &);
std::string to_string(processing::cast::output const &);
}

#include "yas_processing_cast_module_private.h"
