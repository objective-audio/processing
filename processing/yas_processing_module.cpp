//
//  yas_processing_module.cpp
//

#include "yas_processing_module.h"
#include "yas_processing_connector.h"

using namespace yas;

#pragma mark - utility

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

#pragma mark - module::impl

struct processing::module::impl : base::impl {
    impl(processors_t &&processors) : _processors(std::move(processors)) {
    }

    connector_map_t &input_connectors() {
        return this->_input_connectors;
    }

    connector_map_t &output_connectors() {
        return this->_output_connectors;
    }

    processors_t &processors() {
        return this->_processors;
    }

    void process(processing::time::range const &time_range, stream &stream) {
        for (auto &processor : this->_processors) {
            if (processor) {
                processor(time_range, this->_input_connectors, this->_output_connectors, stream);
            }
        }
    }

   private:
    processors_t _processors;
    connector_map_t _input_connectors;
    connector_map_t _output_connectors;
};

#pragma mark - module

processing::module::module(processors_t processors) : base(std::make_shared<impl>(std::move(processors))) {
}

processing::module::module(std::nullptr_t) : base(nullptr) {
}

void processing::module::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}

processing::connector_map_t const &processing::module::input_connectors() const {
    return this->impl_ptr<impl>()->input_connectors();
}

processing::connector_map_t const &processing::module::output_connectors() const {
    return this->impl_ptr<impl>()->output_connectors();
}

void processing::module::connect_input(connector_index_t const co_idx, channel_index_t const ch_idx) {
    connect(this->impl_ptr<impl>()->input_connectors(), co_idx, ch_idx);
}

void processing::module::connect_output(connector_index_t const co_idx, channel_index_t const ch_idx) {
    connect(this->impl_ptr<impl>()->output_connectors(), co_idx, ch_idx);
}

void processing::module::disconnect_input(connector_index_t const idx) {
    disconnect(this->impl_ptr<impl>()->input_connectors(), idx);
}

void processing::module::disconnect_output(connector_index_t const idx) {
    disconnect(this->impl_ptr<impl>()->output_connectors(), idx);
}

processing::module::processors_t const &processing::module::processors() const {
    return this->impl_ptr<impl>()->processors();
}
