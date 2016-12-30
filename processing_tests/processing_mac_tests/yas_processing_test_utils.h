//
//  yas_processing_test_utils.h
//

#pragma once

#include "yas_processing.h"
#include <functional>

namespace yas {
namespace test {
    using process_f = std::function<void(processing::stream &)>;

    struct processor_impl;

    processing::processor make_processor(process_f);
}
}
