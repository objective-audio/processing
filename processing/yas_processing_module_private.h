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

    virtual void process(processing::time_range const &current_time_range, stream &stream) {
        auto const module = cast<processing::module>();

        for (auto &processor : _processors) {
            if (!stream.time_range().is_contain(current_time_range)) {
                throw "out of range.";
            }
            processor.process(current_time_range, module, stream);
        }
    }

   private:
    processors_t _processors;
    connector_map_t _input_connectors;
    connector_map_t _output_connectors;
};
}
