//
//  yas_processing_module.cpp
//

#include "yas_processing_module.h"
#include <cpp_utils/yas_stl_utils.h>
#include "yas_processing_connector.h"

using namespace yas;

#pragma mark - utility

namespace yas::proc {
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
}  // namespace yas::proc

#pragma mark - module::impl

struct proc::module::impl : base::impl {
    impl(make_processors_t &&handler, connector_map_t &&input_connectors, connector_map_t &&output_connectors)
        : _make_handler(std::move(handler)),
          _processors(_make_handler()),
          _input_connectors(std::move(input_connectors)),
          _output_connectors(std::move(output_connectors)) {
    }

    impl(make_processors_t &&handler) : impl(std::move(handler), connector_map_t{}, connector_map_t{}) {
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

    void process(proc::time::range const &time_range, stream &stream) {
        for (auto &processor : this->_processors) {
            if (processor) {
                processor(time_range, this->_input_connectors, this->_output_connectors, stream);
            }
        }
    }

    module copy() {
        if (!this->_make_handler) {
            throw std::runtime_error("make_handler is null.");
        }
        return module{this->_make_handler, this->_input_connectors, this->_output_connectors};
    }

   private:
    make_processors_t _make_handler = nullptr;
    processors_t _processors;
    connector_map_t _input_connectors;
    connector_map_t _output_connectors;
};

#pragma mark - module

proc::module::module(make_processors_t handler) : base(std::make_shared<impl>(std::move(handler))) {
}

proc::module::module(make_processors_t handler, connector_map_t input_connectors, connector_map_t output_connectors)
    : base(std::make_shared<impl>(std::move(handler), std::move(input_connectors), std::move(output_connectors))) {
}

proc::module::module(std::nullptr_t) : base(nullptr) {
}

void proc::module::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}

proc::connector_map_t const &proc::module::input_connectors() const {
    return this->impl_ptr<impl>()->input_connectors();
}

proc::connector_map_t const &proc::module::output_connectors() const {
    return this->impl_ptr<impl>()->output_connectors();
}

void proc::module::connect_input(connector_index_t const co_idx, channel_index_t const ch_idx) {
    connect(this->impl_ptr<impl>()->input_connectors(), co_idx, ch_idx);
}

void proc::module::connect_output(connector_index_t const co_idx, channel_index_t const ch_idx) {
    connect(this->impl_ptr<impl>()->output_connectors(), co_idx, ch_idx);
}

void proc::module::disconnect_input(connector_index_t const idx) {
    disconnect(this->impl_ptr<impl>()->input_connectors(), idx);
}

void proc::module::disconnect_output(connector_index_t const idx) {
    disconnect(this->impl_ptr<impl>()->output_connectors(), idx);
}

proc::module::processors_t const &proc::module::processors() const {
    return this->impl_ptr<impl>()->processors();
}

proc::module proc::module::copy() const {
    return this->impl_ptr<impl>()->copy();
}

std::vector<proc::module> proc::copy(std::vector<proc::module> const &modules) {
    return to_vector<proc::module>(modules, [](proc::module const &module) { return module.copy(); });
}
