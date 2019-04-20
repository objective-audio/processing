//
//  yas_processing_module.h
//

#pragma once

#include <chaining/yas_chaining_umbrella.h>
#include <cpp_utils/yas_base.h>
#include <vector>
#include "yas_processing_connector.h"
#include "yas_processing_processor.h"
#include "yas_processing_stream.h"
#include "yas_processing_time.h"

namespace yas::proc {
class module : public base {
   public:
    class impl;

    using processors_t = std::vector<processor_f>;
    using make_processors_t = std::function<std::vector<processor_f>()>;

    explicit module(make_processors_t);
    module(std::nullptr_t);

    void process(time::range const &, stream &);

    connector_map_t const &input_connectors() const;
    connector_map_t const &output_connectors() const;
    void connect_input(connector_index_t const, channel_index_t const);
    void connect_output(connector_index_t const, channel_index_t const);
    void disconnect_input(connector_index_t const);
    void disconnect_output(connector_index_t const);

    processors_t const &processors() const;

    module copy() const;
};

using module_vector_t = std::vector<module>;
using module_vector_holder_t = chaining::vector::holder<module>;

namespace module_vector {
    using event_type_t = chaining::event_type;
    using inserted_event_t = chaining::vector::inserted_event<module>;
    using erased_event_t = chaining::vector::erased_event<module>;
}  // namespace module_vector

module_vector_t copy(module_vector_t const &);
}  // namespace yas::proc
