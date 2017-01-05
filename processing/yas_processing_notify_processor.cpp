//
//  yas_processing_notify_processor.cpp
//

#include "yas_processing_notify_processor.h"
#include "yas_processing_processor.h"

using namespace yas;

namespace yas {
namespace processing {
    struct notify_processor_impl : processor::impl {
        notify_processor_impl(processing::notify_process_f &&handler) : _handler(std::move(handler)) {
        }

        void process(time::range const &current_time_range, connector_map_t const &, connector_map_t const &,
                     stream &) override {
            if (_handler) {
                _handler(current_time_range);
            }
        }

       private:
        processing::notify_process_f _handler;
    };
}
}

processing::processor processing::make_notify_processor(processing::notify_process_f handler) {
    return processing::processor{std::make_shared<notify_processor_impl>(std::move(handler))};
}
