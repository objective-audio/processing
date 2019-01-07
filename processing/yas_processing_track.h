//
//  yas_processing_track.h
//

#pragma once

#include <chaining/yas_chaining_umbrella.h>
#include <optional>
#include "yas_processing_time.h"

namespace yas::proc {
class module;
class stream;

using track_event_type = chaining::multimap::event_type;
using track_event_t = chaining::multimap::event<time::range, module>;

class track : public chaining::sender<track_event_t> {
    class impl;

   public:
    using modules_map_t = std::multimap<time::range, module>;

    track();
    track(std::nullptr_t);

    modules_map_t const &modules() const;
    modules_map_t &modules();

    std::optional<time::range> total_range() const;

    void insert_module(time::range, module);
    void erase_module(module const &);

    void process(time::range const &, stream &);

    chaining::chain_sync_t<track_event_t> chain();
};
}  // namespace yas::proc
