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

class track : public chaining::sender<chaining::event> {
    class impl;

   public:
    using modules_map_t = std::multimap<time::range, module>;
    using event_t = chaining::event;
    using fetched_event_t = chaining::multimap::fetched_event<time::range, module>;
    using inserted_event_t = chaining::multimap::inserted_event<time::range, module>;
    using erased_event_t = chaining::multimap::erased_event<time::range, module>;

    track();
    track(std::nullptr_t);

    modules_map_t const &modules() const;
    modules_map_t &modules();

    std::optional<time::range> total_range() const;

    void insert_module(time::range, module);
    void erase_module(module const &);

    void process(time::range const &, stream &);

    chaining::chain_sync_t<event_t> chain();
};
}  // namespace yas::proc
