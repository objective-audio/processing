//
//  yas_processing_stream.cpp
//

#include "yas_processing_stream.h"
#include "yas_processing_channel.h"
#include <unordered_map>

using namespace yas;

#pragma mark - processing::stream::impl

struct processing::stream::impl : base::impl {
    std::unordered_map<channel_index_t, processing::channel> _channels;
};

#pragma mark - processing::stream

processing::stream::stream() : base(std::make_shared<impl>()) {
}

processing::stream::stream(std::nullptr_t) : base(nullptr) {
}

void processing::stream::add_channel(channel_index_t const channel) {
    impl_ptr<impl>()->_channels.emplace(channel, processing::channel());
}

bool processing::stream::has_channel(channel_index_t const channel) {
    return impl_ptr<impl>()->_channels.count(channel) > 0;
}

processing::channel const &processing::stream::channel(channel_index_t const channel) const {
    return impl_ptr<impl>()->_channels.at(channel);
}

processing::channel &processing::stream::channel(channel_index_t const channel) {
    return impl_ptr<impl>()->_channels.at(channel);
}

std::size_t processing::stream::channel_count() const {
    return impl_ptr<impl>()->_channels.size();
}
