//
//  yas_processing_processor.cpp
//

#include "yas_processing_processor.h"

using namespace yas;

processing::processor::processor(std::shared_ptr<impl> &&impl) : base(std::move(impl)) {
}

processing::processor::processor(std::nullptr_t) : base(nullptr) {
}

void processing::processor::process(time_range const &time_range, module const &module, stream &stream) {
    impl_ptr<impl>()->process(time_range, module, stream);
}