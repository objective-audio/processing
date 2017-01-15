//
//  yas_processing_module.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_connector.h"
#include "yas_processing_processor.h"
#include "yas_processing_time.h"
#include "yas_processing_stream.h"
#include <vector>

namespace yas {
namespace processing {
    class module : public base {
       public:
        class impl;

        using processors_t = std::vector<processor_f>;

        explicit module(processors_t);
        module(std::nullptr_t);

        void process(time::range const &, stream &);

        connector_map_t const &input_connectors() const;
        connector_map_t const &output_connectors() const;
        void connect_input(connector_index_t const, channel_index_t const);
        void connect_output(connector_index_t const, channel_index_t const);
        void disconnect_input(connector_index_t const);
        void disconnect_output(connector_index_t const);

        processors_t const &processors() const;
    };
}
}

#include "yas_processing_module_private.h"
