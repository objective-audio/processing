//
//  yas_processing_notify_processor.cpp
//

#include "yas_processing_notify_processor.h"
#include "yas_processing_processor.h"

using namespace yas;

processing::processor_f processing::make_notify_processor(processing::notify_process_f handler) {
    return [handler = std::move(handler)](time::range const &current_time_range, connector_map_t const &,
                                          connector_map_t const &, stream &) {
        if (handler) {
            handler(current_time_range);
        }
    };
}
