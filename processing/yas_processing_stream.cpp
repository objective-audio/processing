//
//  yas_processing_stream.cpp
//

#include "yas_processing_stream.h"
#include "yas_processing_channel.h"
#include <unordered_map>

using namespace yas;

#pragma mark - processing::stream::impl

struct processing::stream::impl : base::impl {
    processing::time_range _time_range;
    std::unordered_map<connector::channel_index_t, processing::channel> _channels;

    impl(processing::time_range &&time_range) : _time_range(std::move(time_range)) {
    }
};

#pragma mark - processing::stream

processing::stream::stream(processing::time_range time_range) : base(std::make_shared<impl>(std::move(time_range))) {
}

processing::stream::stream(std::nullptr_t) : base(nullptr) {
}

processing::time_range const &processing::stream::time_range() const {
    return impl_ptr<impl>()->_time_range;
}

void processing::stream::insert_channel(connector::channel_index_t const channel) {
    impl_ptr<impl>()->_channels.emplace(channel, processing::channel());
}

bool processing::stream::has_channel(connector::channel_index_t const channel) {
    return impl_ptr<impl>()->_channels.count(channel) > 0;
}

processing::channel const &processing::stream::channel(connector::channel_index_t const channel) const {
    return impl_ptr<impl>()->_channels.at(channel);
}

processing::channel &processing::stream::channel(connector::channel_index_t const channel) {
    return impl_ptr<impl>()->_channels.at(channel);
}

std::size_t processing::stream::channel_count() const {
    return impl_ptr<impl>()->_channels.size();
}
