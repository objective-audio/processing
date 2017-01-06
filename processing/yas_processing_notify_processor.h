//
//  yas_processing_notify_processor.h
//

#pragma once

#include "yas_processing_types.h"
#include "yas_processing_time.h"
#include "yas_processing_processor.h"
#include <functional>
#include <string>

namespace yas {
namespace processing {
    using notify_process_f =
        std::function<void(processing::time::range const &)>;

    processor_f make_notify_processor(notify_process_f);
}
}
