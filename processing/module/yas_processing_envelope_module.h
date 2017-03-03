//
//  yas_processing_envelope_module.h
//

#pragma once

#include "yas_processing_types.h"
#include <map>

namespace yas {
namespace processing {
    class module;

    namespace envelope {
        template <typename T>
        using anchors_t = std::map<frame_index_t, T>;

        enum class output : connector_index_t {
            value,
        };

        template <typename T>
        module make_signal_module(anchors_t<T>, frame_index_t const offset);
    }
}

void connect(processing::module &, processing::envelope::output const &, processing::channel_index_t const &);

std::string to_string(processing::envelope::output const &);
}
