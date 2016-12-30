//
//  yas_processing_module_private.h
//

#pragma once

namespace yas {
struct processing::module::impl : base::impl {
    impl(args &&args) : _args(std::move(args)) {
    }

    connector_map_t &input_connectors() {
        return _input_connectors;
    }

    connector_map_t &output_connectors() {
        return _output_connectors;
    }

    processors_t &processors() {
        return _args.processors;
    }

    processing::time_range &time_range() {
        return _args.time_range;
    }

    virtual void process(stream &stream) {
        auto const current_time_range_opt = time_range().intersect(stream.time_range());
        if (!current_time_range_opt) {
            return;
        }

        auto const module = cast<processing::module>();
        auto const &current_time_range = *current_time_range_opt;

        for (auto &processor : _args.processors) {
            processor.process(module, current_time_range, stream);
        }
    }

   private:
    args _args;
    connector_map_t _input_connectors;
    connector_map_t _output_connectors;
};
}
