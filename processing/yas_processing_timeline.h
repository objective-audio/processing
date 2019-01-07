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

using timeline_event_type = chaining::map::event_type;
using timeline_event_t = chaining::map::event<track_index_t, track>;

class timeline : public chaining::sender<timeline_event_t> {
    class impl;

   public:
    using track_map_t = std::map<track_index_t, proc::track>;
    using offline_process_f = std::function<void(time::range const &, stream const &, bool &stop)>;

    timeline();
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

    /// 1回だけ処理する
    void process(time::range const &, stream &);
    /// スライス分の処理を繰り返す
    void process(time::range const &, sync_source const &, offline_process_f);

    chaining::chain_sync_t<timeline_event_t> chain();
};
}  // namespace yas::proc
