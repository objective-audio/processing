//
//  yas_processing_module.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_connector.h"
#include "yas_processing_processor.h"
#include "yas_processing_time_range.h"
#include "yas_processing_stream.h"
#include <vector>
#include <unordered_map>

namespace yas {
namespace processing {
//    class stream;
//    class time_range;
//    class connector;
//    class processor;

    class module : public base {
       public:
        class impl;
        
        using connector_map_t = std::unordered_map<std::string, connector>;
        using processors_t = std::vector<processor>;

        module(time_range, processors_t);
        module(std::nullptr_t);

        void process(stream &);

        connector_map_t const &input_connectors() const;
        connector_map_t const &output_connectors() const;
        void connect_input(std::string const &key, int64_t const ch);
        void connect_output(std::string const &key, int64_t const ch);
        void disconnect_input(std::string const &key);
        void disconnect_output(std::string const &key);

        processors_t const &processors() const;
        processors_t &processors();
        void insert_processor(processor);

        time_range const &time_range() const;
    };
}
}

#include "yas_processing_module_private.h"
