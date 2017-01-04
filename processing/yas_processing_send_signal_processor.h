//
//  yas_processing_send_signal_processor.h
//

#pragma once

#include "yas_processing_types.h"
#include "yas_processing_time_range.h"
#include <functional>
#include <string>

namespace yas {
namespace processing {
    class processor;

    template <typename T>
    using send_signal_f =
        std::function<void(processing::time::range const &, channel_index_t const, std::string const &, T *const)>;

    template <typename T>
    processor make_send_signal_processor(send_signal_f<T>);
}
}
