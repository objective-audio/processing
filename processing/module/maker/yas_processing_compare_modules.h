//
//  yas_processing_compare_modules.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_ptr.h>

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
}  // namespace yas::proc::compare

namespace yas::proc {
template <typename T>
[[nodiscard]] module_ptr make_signal_module(compare::kind const);

template <typename T>
[[nodiscard]] module_ptr make_number_module(compare::kind const);
}  // namespace yas::proc

namespace yas {
void connect(proc::module_ptr const &, proc::compare::input const &, proc::channel_index_t const &);
void connect(proc::module_ptr const &, proc::compare::output const &, proc::channel_index_t const &);

[[nodiscard]] std::string to_string(proc::compare::input const &);
[[nodiscard]] std::string to_string(proc::compare::output const &);
}  // namespace yas
