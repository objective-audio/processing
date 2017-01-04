//
//  yas_processing_module.cpp
//

#include "yas_processing_module.h"
#include "yas_processing_connector.h"

using namespace yas;

namespace yas {
namespace processing {
    static void connect(connector_map_t &connectors, std::string const &key, channel_index_t const ch_idx) {
        if (connectors.count(key) == 0) {
            connectors.erase(key);
        }
        connectors.emplace(key, connector{.channel_index = ch_idx});
    }

    static void disconnect(connector_map_t &connectors, std::string const &key) {
        if (connectors.count(key) > 0) {
            connectors.erase(key);
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

void processing::module::connect_input(std::string const &key, channel_index_t const ch) {
    connect(impl_ptr<impl>()->input_connectors(), key, ch);
}

void processing::module::connect_output(std::string const &key, channel_index_t const ch) {
    connect(impl_ptr<impl>()->output_connectors(), key, ch);
}

void processing::module::disconnect_input(std::string const &key) {
    disconnect(impl_ptr<impl>()->input_connectors(), key);
}

void processing::module::disconnect_output(std::string const &key) {
    disconnect(impl_ptr<impl>()->output_connectors(), key);
}

processing::module::processors_t const &processing::module::processors() const {
    return impl_ptr<impl>()->processors();
}
