//
//  yas_processing_processor.cpp
//

#include "yas_processing_processor.h"

using namespace yas;

processing::processor::processor(std::shared_ptr<impl> &&impl) : base(std::move(impl)) {
}

processing::processor::processor(std::nullptr_t) : base(nullptr) {
}

void processing::processor::process(module const &module, time_range const &time_range, stream &stream) {
    impl_ptr<impl>()->process(module, time_range, stream);
}
