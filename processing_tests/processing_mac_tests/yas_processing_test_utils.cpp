//
//  yas_processing_test_utils.cpp
//

#include "yas_processing_test_utils.h"

using namespace yas;

struct test::processor_impl : processing::processor::impl {
    processor_impl(process_f &&handler) : _handler(std::move(handler)) {
    }
    
    virtual void process(processing::module const &, processing::time_range const &,
                         processing::stream &stream) override {
        if (_handler) {
            _handler(stream);
        }
    }
    
private:
    process_f _handler;
};

processing::processor test::make_processor(process_f handler) {
    return processing::processor{std::make_shared<test::processor_impl>(std::move(handler))};
}
