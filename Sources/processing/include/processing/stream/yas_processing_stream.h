//
//  yas_processing_stream.h
//

#pragma once

#include <processing/channel/yas_processing_channel.h>
#include <processing/common/yas_processing_common_types.h>
#include <processing/sync_source/yas_processing_sync_source.h>
#include <processing/time/yas_processing_time.h>

namespace yas::proc {
struct stream final {
    explicit stream(sync_source const &);
    explicit stream(sync_source &&);

    stream(stream &&);
    stream(stream const &);

    [[nodiscard]] sync_source const &sync_source() const;

    proc::channel &add_channel(channel_index_t const);
    proc::channel &add_channel(channel_index_t const, channel::events_map_t);
    void remove_channel(channel_index_t const);
    [[nodiscard]] bool has_channel(channel_index_t const);
    [[nodiscard]] proc::channel const &channel(channel_index_t const) const;
    [[nodiscard]] proc::channel &channel(channel_index_t const);
    [[nodiscard]] std::size_t channel_count() const;
    [[nodiscard]] std::map<channel_index_t, proc::channel> const &channels() const;

   private:
    proc::sync_source _sync_source;
    std::map<channel_index_t, proc::channel> _channels;

    stream &operator=(stream &&) = delete;
    stream &operator=(stream const &) = delete;
};
}  // namespace yas::proc
