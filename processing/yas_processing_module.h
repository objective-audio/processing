//
//  yas_processing_module.h
//

#pragma once

#include <chaining/yas_chaining_umbrella.h>
#include <vector>
#include "yas_processing_connector.h"
#include "yas_processing_processor.h"

#include "yas_processing_stream.h"
#include "yas_processing_time.h"

namespace yas::proc {
struct module final {
    using processors_t = std::vector<processor_f>;
    using make_processors_t = std::function<std::vector<processor_f>()>;

    void process(time::range const &, stream &);

    connector_map_t const &input_connectors() const;
    connector_map_t const &output_connectors() const;
    void connect_input(connector_index_t const, channel_index_t const);
    void connect_output(connector_index_t const, channel_index_t const);
    void disconnect_input(connector_index_t const);
    void disconnect_output(connector_index_t const);

    processors_t const &processors() const;

    module_ptr copy() const;

    static module_ptr make_shared(make_processors_t);
    static module_ptr make_shared(make_processors_t, connector_map_t input_connectors,
                                  connector_map_t output_connectors);

   private:
    make_processors_t const _make_handler;
    processors_t const _processors;
    connector_map_t _input_connectors;
    connector_map_t _output_connectors;

    module(make_processors_t &&, connector_map_t &&input_connectors, connector_map_t &&output_connectors);
};

using module_vector_t = std::vector<module_ptr>;
using module_vector_holder_t = chaining::vector::holder<module_ptr>;
using module_vector_holder_ptr_t = chaining::vector::holder_ptr<module_ptr>;

namespace module_vector {
    using event_type_t = chaining::event_type;
    using inserted_event_t = chaining::vector::inserted_event<module_ptr>;
    using erased_event_t = chaining::vector::erased_event<module_ptr>;
}  // namespace module_vector

module_vector_t copy(module_vector_t const &);
}  // namespace yas::proc
