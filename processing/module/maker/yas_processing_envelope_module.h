//
//  yas_processing_envelope_module.h
//

#pragma once

#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_ptr.h>

#include <map>

namespace yas::proc {
/// エンベロープを生成するモジュール
namespace envelope {
    template <typename T>
    using anchors_t = std::map<frame_index_t, T>;

    enum class output : connector_index_t {
        value,
    };

    template <typename T>
    module_ptr make_signal_module(anchors_t<T>, frame_index_t const offset);
}  // namespace envelope
}  // namespace yas::proc

namespace yas {
void connect(proc::module_ptr const &, proc::envelope::output const &, proc::channel_index_t const &);

std::string to_string(proc::envelope::output const &);
}  // namespace yas
