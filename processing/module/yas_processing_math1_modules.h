//
//  yas_processing_math1_modules.h
//

#pragma once

#include "yas_processing_types.h"
#include <string>

namespace yas::processing {
class module;

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
}

template <typename T>
module make_signal_module(math1::kind const);

template <typename T>
module make_number_module(math1::kind const);
}

namespace yas {
void connect(processing::module &, processing::math1::input const &, processing::channel_index_t const &);
void connect(processing::module &, processing::math1::output const &, processing::channel_index_t const &);

std::string to_string(processing::math1::kind const &);
std::string to_string(processing::math1::input const &);
std::string to_string(processing::math1::output const &);
}
