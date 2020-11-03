//
//  yas_processing_math1_modules.h
//

#pragma once

#include <processing/yas_processing_types.h>

#include <string>

namespace yas::proc {
/// パラメータをひとつだけ受け取る算術関数の結果を生成するモジュール
namespace math1 {
    enum class kind {
        sin,
        cos,
        tan,
        asin,
        acos,
        atan,

        sinh,
        cosh,
        tanh,
        asinh,
        acosh,
        atanh,

        exp,
        exp2,
        expm1,
        log,
        log10,
        log1p,
        log2,

        sqrt,
        cbrt,
        abs,

        ceil,
        floor,
        trunc,
        round,
    };

    enum class input : connector_index_t {
        parameter,
    };

    enum class output : connector_index_t {
        result,
    };
}  // namespace math1

template <typename T>
module_ptr make_signal_module(math1::kind const);

template <typename T>
module_ptr make_number_module(math1::kind const);
}  // namespace yas::proc

namespace yas {
void connect(proc::module_ptr const &, proc::math1::input const &, proc::channel_index_t const &);
void connect(proc::module_ptr const &, proc::math1::output const &, proc::channel_index_t const &);

std::string to_string(proc::math1::kind const &);
std::string to_string(proc::math1::input const &);
std::string to_string(proc::math1::output const &);
}  // namespace yas
