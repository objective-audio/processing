//
//  yas_processing_routing_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas::proc {
class module;

/// ルーティングするモジュール
namespace routing {
    enum class kind {
        move,
        copy,
    };

    enum class input : connector_index_t {
        value,
    };

    enum class output : connector_index_t {
        value,
    };
}

template <typename T>
module make_signal_module(routing::kind const);

template <typename T>
module make_number_module(routing::kind const);
}

namespace yas {
void connect(proc::module &, proc::routing::input const &, proc::channel_index_t const &);
void connect(proc::module &, proc::routing::output const &, proc::channel_index_t const &);

std::string to_string(proc::routing::input const &);
std::string to_string(proc::routing::output const &);
}
