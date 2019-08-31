//
//  yas_processing_constant_module.h
//

#pragma once

#include "yas_processing_ptr.h"
#include "yas_processing_types.h"

namespace yas::proc {
class module;

/// 固定値を生成するモジュール
namespace constant {
    enum class output : connector_index_t {
        value,
    };
}

template <typename T>
module_ptr make_signal_module(T);

template <typename T>
module_ptr make_number_module(T);
}  // namespace yas::proc
