//
//  yas_processing_timeline.h
//

#pragma once

#include <chaining/yas_chaining_umbrella.h>
#include <cpp_utils/yas_base.h>
#include <functional>
#include <optional>
#include "yas_processing_time.h"
#include "yas_processing_types.h"

namespace yas::proc {
class track;
class stream;
class sync_source;

class timeline : public chaining::sender<chaining::event> {
    class impl;

   public:
    using track_map_t = std::map<track_index_t, proc::track>;
    using process_track_f =
        std::function<continuation(time::range const &, stream const &, std::optional<track_index_t> const &)>;
    using process_f = std::function<continuation(time::range const &, stream const &)>;
    using event_type_t = chaining::event_type;
    using event_t = chaining::event;
    using fetched_event_t = chaining::map::fetched_event<track_index_t, proc::track>;
    using inserted_event_t = chaining::map::inserted_event<track_index_t, proc::track>;
    using erased_event_t = chaining::map::erased_event<track_index_t, proc::track>;
    using relayed_event_t = chaining::map::relayed_event<track_index_t, proc::track>;

    timeline();
    timeline(track_map_t &&);
    timeline(std::nullptr_t);

    track_map_t const &tracks() const;
    track_map_t &tracks();

    bool insert_track(track_index_t const, track);
    void erase_track(track_index_t const);
    std::size_t track_count() const;
    bool has_track(track_index_t const) const;
    proc::track const &track(track_index_t const) const;
    proc::track &track(track_index_t const);

    std::optional<time::range> total_range() const;

    timeline copy() const;

    /// 1回だけ処理する
    void process(time::range const &, stream &);
    /// スライス分の処理を繰り返す
    void process(time::range const &, sync_source const &, process_f const);
    void process(time::range const &, sync_source const &, process_track_f const);

    chaining::chain_sync_t<event_t> chain() const;
};

timeline::track_map_t copy_tracks(timeline::track_map_t const &);
}  // namespace yas::proc
