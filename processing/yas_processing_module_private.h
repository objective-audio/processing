//
//  yas_processing_module_private.h
//

#pragma once

namespace yas {
struct processing::module::impl : base::impl {
    impl(processors_t &&processors) : _processors(std::move(processors)) {
    }

    connector_map_t &input_connectors() {
        return _input_connectors;
    }

    connector_map_t &output_connectors() {
        return _output_connectors;
    }

    processors_t &processors() {
        return _processors;
    }

    void process(processing::time::range const &time_range, stream &stream) {
        for (auto &processor : _processors) {
            if (processor) {
                processor(time_range, _input_connectors, _output_connectors, stream);
            }
        }
    }

   private:
    processors_t _processors;
    connector_map_t _input_connectors;
    connector_map_t _output_connectors;
};
}
