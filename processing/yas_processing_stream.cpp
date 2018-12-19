//
//  yas_processing_stream.cpp
//

#include "yas_processing_stream.h"
#include <map>
#include "yas_processing_channel.h"
#include "yas_processing_sync_source.h"

using namespace yas;

#pragma mark - proc::stream::impl

struct proc::stream::impl : base::impl {
    proc::sync_source _sync_source;
    std::map<channel_index_t, proc::channel> _channels;

    impl(proc::sync_source sync_src) : _sync_source(std::move(sync_src)) {
    }
};

#pragma mark - proc::stream

proc::stream::stream(proc::sync_source sync_src) : base(std::make_shared<impl>(std::move(sync_src))) {
}

proc::stream::stream(std::nullptr_t) : base(nullptr) {
}

proc::sync_source const &proc::stream::sync_source() const {
    return impl_ptr<impl>()->_sync_source;
}

proc::channel &proc::stream::add_channel(channel_index_t const ch_idx) {
    auto &channels = this->impl_ptr<impl>()->_channels;
    if (channels.count(ch_idx) == 0) {
        channels.emplace(ch_idx, proc::channel{});
    }
    return channels.at(ch_idx);
}

proc::channel &proc::stream::add_channel(channel_index_t const ch_idx, channel::events_map_t events) {
    auto &channels = this->impl_ptr<impl>()->_channels;
    if (channels.count(ch_idx) > 0) {
        throw "channel exists.";
    }
    channels.emplace(ch_idx, proc::channel{std::move(events)});
    return channels.at(ch_idx);
}

void proc::stream::remove_channel(channel_index_t const ch_idx) {
    auto &channels = this->impl_ptr<impl>()->_channels;
    if (channels.count(ch_idx) > 0) {
        channels.erase(ch_idx);
    }
}

bool proc::stream::has_channel(channel_index_t const channel) {
    return this->impl_ptr<impl>()->_channels.count(channel) > 0;
}

proc::channel const &proc::stream::channel(channel_index_t const channel) const {
    return this->impl_ptr<impl>()->_channels.at(channel);
}

proc::channel &proc::stream::channel(channel_index_t const channel) {
    return this->impl_ptr<impl>()->_channels.at(channel);
}

std::size_t proc::stream::channel_count() const {
    return this->impl_ptr<impl>()->_channels.size();
}

std::map<proc::channel_index_t, proc::channel> const &proc::stream::channels() const {
    return this->impl_ptr<impl>()->_channels;
}
