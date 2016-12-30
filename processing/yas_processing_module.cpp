//
//  yas_processing_module.cpp
//

#include "yas_processing_module.h"
#include "yas_processing_connector.h"
#include "yas_processing_time_range.h"

using namespace yas;

namespace yas {
namespace processing {
    static void connect(module::connector_map_t &connectors, std::string const &key, int64_t const ch_idx) {
        if (connectors.count(key) == 0) {
            connectors.erase(key);
        }
        connectors.emplace(key, connector{.channel_index = ch_idx});
    }

    static void disconnect(module::connector_map_t &connectors, std::string const &key) {
        if (connectors.count(key) > 0) {
            connectors.erase(key);
        }
    }
}
}

processing::module::module(std::shared_ptr<impl> &&impl) : base(std::move(impl)) {
}

processing::module::module(processing::time_range time_range, std::vector<processor> processors)
    : base(std::make_shared<impl>(std::move(time_range), std::move(processors))) {
}

processing::module::module(std::nullptr_t) : base(nullptr) {
}

void processing::module::process(stream &stream) {
    impl_ptr<impl>()->process(stream);
}

processing::module::connector_map_t const &processing::module::input_connectors() const {
    return impl_ptr<impl>()->input_connectors();
}

processing::module::connector_map_t const &processing::module::output_connectors() const {
    return impl_ptr<impl>()->output_connectors();
}

void processing::module::connect_input(std::string const &key, int64_t const ch) {
    connect(impl_ptr<impl>()->input_connectors(), key, ch);
}

void processing::module::connect_output(std::string const &key, int64_t const ch) {
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

processing::module::processors_t &processing::module::processors() {
    return impl_ptr<impl>()->processors();
}

void processing::module::insert_processor(processing::processor processor) {
    impl_ptr<impl>()->processors().emplace_back(std::move(processor));
}

processing::time_range const &processing::module::time_range() const {
    return impl_ptr<impl>()->time_range();
}
