//
//  yas_processing_envelope_module.h
//

#pragma once

#include <map>
#include "yas_processing_types.h"

namespace yas::proc {
class module;

/// エンベロープを生成するモジュール
namespace envelope {
    template <typename T>
    using anchors_t = std::map<frame_index_t, T>;

    enum class output : connector_index_t {
        value,
    };

    template <typename T>
    module make_signal_module(anchors_t<T>, frame_index_t const offset);
}  // namespace envelope
}  // namespace yas::proc

namespace yas {
void connect(proc::module &, proc::envelope::output const &, proc::channel_index_t const &);

std::string to_string(proc::envelope::output const &);
}  // namespace yas
