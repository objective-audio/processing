//
//  yas_processing_stream.h
//

#pragma once

#include "yas_processing_channel.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_time.h"
#include "yas_processing_types.h"

namespace yas::proc {
struct stream final {
    explicit stream(sync_source const &);
    explicit stream(sync_source &&);

    stream(stream &&) = default;
    stream &operator=(stream &&) = default;

    sync_source const &sync_source() const;

    proc::channel &add_channel(channel_index_t const);
    proc::channel &add_channel(channel_index_t const, channel::events_map_t);
    void remove_channel(channel_index_t const);
    bool has_channel(channel_index_t const);
    proc::channel const &channel(channel_index_t const) const;
    proc::channel &channel(channel_index_t const);
    std::size_t channel_count() const;
    std::map<channel_index_t, proc::channel> const &channels() const;

   private:
    proc::sync_source _sync_source;
    std::map<channel_index_t, proc::channel> _channels;

    stream(stream const &) = delete;
    stream &operator=(stream const &) = delete;
};
}  // namespace yas::proc
