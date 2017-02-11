//
//  yas_processing_stream.cpp
//

#include "yas_processing_stream.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_channel.h"
#include <unordered_map>

using namespace yas;

#pragma mark - processing::stream::impl

struct processing::stream::impl : base::impl {
    processing::sync_source _sync_source;
    std::unordered_map<channel_index_t, processing::channel> _channels;

    impl(processing::sync_source sync_src) : _sync_source(std::move(sync_src)) {
    }
};

#pragma mark - processing::stream

processing::stream::stream(processing::sync_source sync_src) : base(std::make_shared<impl>(std::move(sync_src))) {
}

processing::stream::stream(std::nullptr_t) : base(nullptr) {
}

processing::sync_source const &processing::stream::sync_source() const {
    return impl_ptr<impl>()->_sync_source;
}

processing::channel &processing::stream::add_channel(channel_index_t const ch_idx) {
    auto &channels = this->impl_ptr<impl>()->_channels;
    if (channels.count(ch_idx) == 0) {
        channels.emplace(ch_idx, processing::channel{});
    }
    return channels.at(ch_idx);
}

processing::channel &processing::stream::add_channel(channel_index_t const ch_idx, channel::events_map_t events) {
    auto &channels = this->impl_ptr<impl>()->_channels;
    if (channels.count(ch_idx) > 0) {
        throw "channel exists.";
    }
    channels.emplace(ch_idx, processing::channel{std::move(events)});
    return channels.at(ch_idx);
}

void processing::stream::remove_channel(channel_index_t const ch_idx) {
    auto &channels = this->impl_ptr<impl>()->_channels;
    if (channels.count(ch_idx) > 0) {
        channels.erase(ch_idx);
    }
}

bool processing::stream::has_channel(channel_index_t const channel) {
    return this->impl_ptr<impl>()->_channels.count(channel) > 0;
}

processing::channel const &processing::stream::channel(channel_index_t const channel) const {
    return this->impl_ptr<impl>()->_channels.at(channel);
}

processing::channel &processing::stream::channel(channel_index_t const channel) {
    return this->impl_ptr<impl>()->_channels.at(channel);
}

std::size_t processing::stream::channel_count() const {
    return this->impl_ptr<impl>()->_channels.size();
}
