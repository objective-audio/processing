//
//  yas_processing_timeline.h
//

#pragma once

#include <chaining/yas_chaining_umbrella.h>
#include <functional>
#include <optional>
#include "yas_processing_time.h"
#include "yas_processing_track.h"
#include "yas_processing_types.h"

namespace yas::proc {
struct timeline final : chaining::sender<chaining::map::event> {
    class impl;

    using track_map_t = std::map<track_index_t, proc::track_ptr>;
    using process_track_f =
        std::function<continuation(time::range const &, stream const &, std::optional<track_index_t> const &)>;
    using process_f = std::function<continuation(time::range const &, stream const &)>;
    using event_type_t = chaining::event_type;
    using event_t = chaining::map::event;
    using fetched_event_t = chaining::map::fetched_event<track_index_t, proc::track_ptr>;
    using inserted_event_t = chaining::map::inserted_event<track_index_t, proc::track_ptr>;
    using erased_event_t = chaining::map::erased_event<track_index_t, proc::track_ptr>;
    using relayed_event_t = chaining::map::relayed_event<track_index_t, proc::track_ptr>;

    track_map_t const &tracks() const;

    bool insert_track(track_index_t const, track_ptr const &);
    void erase_track(track_index_t const);
    std::size_t track_count() const;
    bool has_track(track_index_t const) const;
    proc::track_ptr const &track(track_index_t const) const;

    std::optional<time::range> total_range() const;

    timeline_ptr copy() const;

    /// 1回だけ処理する
    void process(time::range const &, stream &);
    /// スライス分の処理を繰り返す
    void process(time::range const &, sync_source const &, process_f const &);
    void process(time::range const &, sync_source const &, process_track_f const &);

    chaining::chain_sync_t<event_t> chain() const;

    static timeline_ptr make_shared();
    static timeline_ptr make_shared(track_map_t &&);

   private:
    std::unique_ptr<impl> _impl;

    timeline(track_map_t &&);

    chaining::chain_unsync_t<event_t> chain_unsync() const override;
    chaining::chain_sync_t<event_t> chain_sync() const override;
    void fetch_for(chaining::any_joint const &joint) const override;
    void broadcast(event_t const &value) const override;
    void erase_joint(std::uintptr_t const key) const override;
    void send_value_to_target(event_t const &value, std::uintptr_t const key) const override;
};

timeline::track_map_t copy_tracks(timeline::track_map_t const &);
}  // namespace yas::proc
