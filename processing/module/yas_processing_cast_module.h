//
//  yas_processing_cast_module.h
//

#pragma once

#include "yas_processing_types.h"
#include <string>

namespace yas::proc {
class module;

/// 型を変換するモジュール
namespace cast {
enum class output : connector_index_t {
  value,
};

enum class input : connector_index_t {
  value,
};

template <typename In, typename Out> module make_signal_module();

template <typename In, typename Out> module make_number_module();
} // namespace cast
} // namespace yas::proc

namespace yas {
void connect(proc::module &, proc::cast::input const &,
             proc::channel_index_t const &);
void connect(proc::module &, proc::cast::output const &,
             proc::channel_index_t const &);

std::string to_string(proc::cast::input const &);
std::string to_string(proc::cast::output const &);
} // namespace yas

#include "yas_processing_cast_module_private.h"
