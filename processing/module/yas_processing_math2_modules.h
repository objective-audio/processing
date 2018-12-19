//
//  yas_processing_math2_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas::proc {
class module;

/// パラメータを2つ受け取る算術関数の結果を生成するモジュール
namespace math2 {
    enum class kind {
        plus,
        minus,
        multiply,
        divide,

        atan2,

        pow,
        hypot,
    };

    enum class output : connector_index_t {
        result,
    };

    enum class input : connector_index_t {
        left,
        right,
    };
}  // namespace math2

template <typename T>
module make_signal_module(math2::kind const);

template <typename T>
module make_number_module(math2::kind const);
}  // namespace yas::proc

namespace yas {
void connect(proc::module &, proc::math2::input const &, proc::channel_index_t const &);
void connect(proc::module &, proc::math2::output const &, proc::channel_index_t const &);

std::string to_string(proc::math2::kind const &);
std::string to_string(proc::math2::input const &);
std::string to_string(proc::math2::output const &);
}  // namespace yas
