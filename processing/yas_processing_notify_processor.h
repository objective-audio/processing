//
//  yas_processing_notify_processor.h
//

#pragma once

#include "yas_processing_types.h"
#include "yas_processing_time.h"
#include <functional>
#include <string>

namespace yas {
namespace processing {
    class processor;

    using notify_process_f =
        std::function<void(processing::time::range const &)>;

    processor make_notify_processor(notify_process_f);
}
}
