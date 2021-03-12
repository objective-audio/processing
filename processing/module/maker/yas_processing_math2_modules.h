//
//  yas_processing_math2_modules.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_ptr.h>

namespace yas::proc {
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
module_ptr make_signal_module(math2::kind const);

template <typename T>
module_ptr make_number_module(math2::kind const);
}  // namespace yas::proc

namespace yas {
void connect(proc::module_ptr const &, proc::math2::input const &, proc::channel_index_t const &);
void connect(proc::module_ptr const &, proc::math2::output const &, proc::channel_index_t const &);

std::string to_string(proc::math2::kind const &);
std::string to_string(proc::math2::input const &);
std::string to_string(proc::math2::output const &);
}  // namespace yas
