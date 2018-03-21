//
//  yas_processing_generator_modules.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas::proc {
class module;

/// 単純な生成モジュール
namespace generator {
    enum class kind {
        second,  /// 1秒につき1.0増えるシグナルを生成する
    };

    enum class output : connector_index_t {
        value,
    };
}

template <typename T>
module make_signal_module(generator::kind const, frame_index_t const offset);
}

namespace yas {
void connect(proc::module &, proc::generator::output const &, proc::channel_index_t const &);

std::string to_string(proc::generator::output const &);
}
