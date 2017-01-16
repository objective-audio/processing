//
//  yas_processing_module.cpp
//

#include "yas_processing_module.h"
#include "yas_processing_connector.h"

using namespace yas;

namespace yas {
namespace processing {
    static void connect(connector_map_t &connectors, connector_index_t const idx, channel_index_t const ch_idx) {
        if (connectors.count(idx) == 0) {
            connectors.erase(idx);
        }
        connectors.emplace(idx, connector{.channel_index = ch_idx});
    }

    static void disconnect(connector_map_t &connectors, connector_index_t const idx) {
        if (connectors.count(idx) > 0) {
            connectors.erase(idx);
        }
    }
}
}

processing::module::module(processors_t processors) : base(std::make_shared<impl>(std::move(processors))) {
}

processing::module::module(std::nullptr_t) : base(nullptr) {
}

void processing::module::process(time::range const &time_range, stream &stream) {
    impl_ptr<impl>()->process(time_range, stream);
}

processing::connector_map_t const &processing::module::input_connectors() const {
    return impl_ptr<impl>()->input_connectors();
}

processing::connector_map_t const &processing::module::output_connectors() const {
    return impl_ptr<impl>()->output_connectors();
}

void processing::module::connect_input(connector_index_t const con_idx, channel_index_t const ch_idx) {
    connect(impl_ptr<impl>()->input_connectors(), con_idx, ch_idx);
}

void processing::module::connect_output(connector_index_t const con_idx, channel_index_t const ch_idx) {
    connect(impl_ptr<impl>()->output_connectors(), con_idx, ch_idx);
}

void processing::module::disconnect_input(connector_index_t const idx) {
    disconnect(impl_ptr<impl>()->input_connectors(), idx);
}

void processing::module::disconnect_output(connector_index_t const idx) {
    disconnect(impl_ptr<impl>()->output_connectors(), idx);
}

processing::module::processors_t const &processing::module::processors() const {
    return impl_ptr<impl>()->processors();
}
