//
//  yas_processing_generator_modules.h
//

#pragma once

#include <processing/yas_processing_types.h>

namespace yas::proc {
/// 単純な生成モジュール
namespace generator {
    enum class kind {
        second,  /// 1秒につき1.0増えるシグナルを生成する
        frame,
    };

    enum class output : connector_index_t {
        value,
    };
}  // namespace generator

template <typename T>
module_ptr make_signal_module(generator::kind const, frame_index_t const offset);
}  // namespace yas::proc

namespace yas {
void connect(proc::module_ptr const &, proc::generator::output const &, proc::channel_index_t const &);

std::string to_string(proc::generator::output const &);
}  // namespace yas
