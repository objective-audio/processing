//
//  yas_processing_compare_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas::proc {
class module;
}

/// 比較をするモジュール
/// 出力はboolean
namespace yas::proc::compare {
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

namespace yas::proc {
template <typename T>
module make_signal_module(compare::kind const);

template <typename T>
module make_number_module(compare::kind const);
}

namespace yas {
void connect(proc::module &, proc::compare::input const &, proc::channel_index_t const &);
void connect(proc::module &, proc::compare::output const &, proc::channel_index_t const &);

std::string to_string(proc::compare::input const &);
std::string to_string(proc::compare::output const &);
}
