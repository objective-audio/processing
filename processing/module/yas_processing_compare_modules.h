//
//  yas_processing_compare_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;

    namespace compare {
        enum class kind {
            is_equal,
            is_not_equal,

            is_greater,
            is_greater_equal,
            is_less,
            is_less_equal,
        };

        enum class input : connector_index_t {
            left,
            right,
        };

        enum class output : connector_index_t {
            result,
        };
    }

    template <typename T>
    module make_signal_module(compare::kind const);

    template <typename T>
    module make_number_module(compare::kind const);
}

void connect(processing::module &, processing::compare::input const &, processing::channel_index_t const &);
void connect(processing::module &, processing::compare::output const &, processing::channel_index_t const &);

std::string to_string(processing::compare::input const &);
std::string to_string(processing::compare::output const &);
}