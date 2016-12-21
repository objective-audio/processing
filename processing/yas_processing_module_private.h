//
//  yas_processing_module_private.h
//

#pragma once

#include "yas_processing_connector.h"
#include "yas_processing_time_range.h"

namespace yas {
struct processing::module::impl : base::impl {
    struct args {
        processing::time_range time_range;
        connector_map_t input_connectors;
        connector_map_t output_connectors;
    };

    impl(args args) : _args(std::move(args)) {
    }

    connector_map_t &input_connectors() {
        return _args.input_connectors;
    }

    connector_map_t &output_connectors() {
        return _args.output_connectors;
    }
    
    processing::time_range &time_range() {
        return _args.time_range;
    }

    virtual int64_t frame_offset() {
        return 0;
    }

    void pre_process(stream &) {
    }

    virtual void process(stream &) = 0;

    void post_process(stream &) {
    }

   private:
    args _args;
};
}
