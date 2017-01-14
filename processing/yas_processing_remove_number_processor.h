//
//  yas_processing_remove_number_processor.h
//

#pragma once

#include "yas_processing_processor.h"
#include <unordered_set>

namespace yas {
namespace processing {
    template <typename T>
    processor_f make_remove_number_processor(std::unordered_set<std::string>);
}
}
