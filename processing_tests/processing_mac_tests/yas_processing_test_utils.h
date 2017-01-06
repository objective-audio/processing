//
//  yas_processing_test_utils.h
//

#pragma once

#include "yas_processing.h"
#include <functional>

namespace yas {
namespace test {
    using process_f = std::function<void(processing::time::range const &, processing::connector_map_t const &,
                                         processing::connector_map_t const &, processing::stream &)>;
}
}
