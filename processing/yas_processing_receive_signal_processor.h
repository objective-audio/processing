//
//  yas_processing_receive_signal_processor.h
//

#pragma once

#include "yas_processing_types.h"
#include <functional>
#include <string>

namespace yas {
namespace processing {
    class time_range;
    class processor;
    
    template <typename T>
    using receive_signal_f =
        std::function<void(processing::time_range const &, channel_index_t const, std::string const &, T const *const)>;

    template <typename T>
    processor make_receive_signal_processor(receive_signal_f<T>);
}
}
