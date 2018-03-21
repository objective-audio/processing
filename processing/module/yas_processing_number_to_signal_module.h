//
//  yas_processing_number_to_signal_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas::processing {
class module;

/// 数値からシグナルへ変換するモジュール
namespace number_to_signal {
    enum class output : connector_index_t {
        signal,
    };

    enum class input : connector_index_t {
        number,
    };
}

template <typename T>
module make_number_to_signal_module();
}

namespace yas {
void connect(processing::module &, processing::number_to_signal::input const &, processing::channel_index_t const &);
void connect(processing::module &, processing::number_to_signal::output const &, processing::channel_index_t const &);

std::string to_string(processing::number_to_signal::input const &);
std::string to_string(processing::number_to_signal::output const &);
}
