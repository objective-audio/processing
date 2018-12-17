//
//  yas_processing_timeline.h
//

#pragma once

#include <functional>
#include <optional>
#include "yas_base.h"
#include "yas_processing_time.h"
#include "yas_processing_types.h"

namespace yas::proc {
class track;
class stream;
class sync_source;

class timeline : public base {
    class impl;

   public:
    using track_map_t = std::map<track_index_t, proc::track>;
    using offline_process_f = std::function<void(time::range const &, stream const &, bool &stop)>;

    timeline();
    timeline(std::nullptr_t);

    track_map_t const &tracks() const;
    track_map_t &tracks();

    proc::track &add_track(track_index_t const);
    void remove_track(track_index_t const);
    std::size_t track_count() const;
    bool has_track(track_index_t const) const;
    proc::track const &track(track_index_t const) const;
    proc::track &track(track_index_t const);

    /// 1回だけ処理する
    void process(time::range const &, stream &);
    /// スライス分の処理を繰り返す
    void process(time::range const &, sync_source const &, offline_process_f);
};
}  // namespace yas::proc
