//
//  yas_processing_module.cpp
//

#include "yas_processing_module.h"
#include "yas_processing_connector.h"
#include "yas_processing_time_range.h"

using namespace yas;

processing::module::module(std::shared_ptr<impl> &&impl) : base(std::move(impl)) {
}

processing::module::module(std::nullptr_t) : base(nullptr) {
}

void processing::module::process(stream &stream) {
    impl_ptr<impl>()->pre_process(stream);
    impl_ptr<impl>()->process(stream);
    impl_ptr<impl>()->post_process(stream);
}

processing::module::connector_map_t const &processing::module::input_connectors() {
    return impl_ptr<impl>()->input_connectors();
}

processing::module::connector_map_t const &processing::module::output_connectors() {
    return impl_ptr<impl>()->output_connectors();
}

void processing::module::connect_input(std::string const &key, int64_t const ch) {
    impl_ptr<impl>()->input_connectors().at(key).channel = ch;
}

void processing::module::connect_output(std::string const &key, int64_t const ch) {
    impl_ptr<impl>()->output_connectors().at(key).channel = ch;
}

void processing::module::disconnect_input(std::string const &key) {
    impl_ptr<impl>()->input_connectors().at(key).channel = std::experimental::nullopt;
}

void processing::module::disconnect_output(std::string const &key) {
    impl_ptr<impl>()->output_connectors().at(key).channel = std::experimental::nullopt;
}

processing::time_range const &processing::module::time_range() const {
    return impl_ptr<impl>()->time_range();
}

int64_t processing::module::frame_offset() const {
    return impl_ptr<impl>()->frame_offset();
}
