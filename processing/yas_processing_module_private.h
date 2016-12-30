//
//  yas_processing_module_private.h
//

#pragma once

namespace yas {
struct processing::module::impl : base::impl {
    impl(processing::time_range time_range) : _time_range(std::move(time_range)) {
    }

    impl(processing::time_range &&time_range, processors_t &&processors)
        : _time_range(std::move(time_range)), _processors(std::move(processors)) {
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

    processing::time_range &time_range() {
        return _time_range;
    }

    virtual void process(stream &stream) {
        auto const current_time_range_opt = time_range().intersect(stream.time_range());
        if (!current_time_range_opt) {
            return;
        }

        auto const module = cast<processing::module>();
        auto const &current_time_range = *current_time_range_opt;

        for (auto &processor : _processors) {
            processor.process(module, current_time_range, stream);
        }
    }

   private:
    processing::time_range _time_range;
    connector_map_t _input_connectors;
    connector_map_t _output_connectors;
    processors_t _processors;
};
}
