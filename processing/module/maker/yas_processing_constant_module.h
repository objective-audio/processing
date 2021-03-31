//
//  yas_processing_constant_module.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_ptr.h>

namespace yas::proc {
/// 固定値を生成するモジュール
namespace constant {
    enum class output : connector_index_t {
        value,
    };
}

template <typename T>
[[nodiscard]] module_ptr make_signal_module(T);

template <typename T>
[[nodiscard]] module_ptr make_number_module(T);
}  // namespace yas::proc
