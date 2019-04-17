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
    using modules_map_t = std::map<time::range, std::vector<module>>;
    using modules_holder_map_t = std::map<time::range, chaining::vector::holder<module>>;
    using event_type_t = chaining::event_type;
    using event_t = chaining::event;
    using fetched_event_t = chaining::map::fetched_event<time::range, chaining::vector::holder<module>>;
    using inserted_event_t = chaining::map::inserted_event<time::range, chaining::vector::holder<module>>;
    using erased_event_t = chaining::map::erased_event<time::range, chaining::vector::holder<module>>;
    using replaced_event_t = chaining::map::replaced_event<time::range, chaining::vector::holder<module>>;
    using relayed_event_t = chaining::map::relayed_event<time::range, chaining::vector::holder<module>>;

    track();
    track(modules_map_t &&);
    track(std::nullptr_t);

    modules_holder_map_t const &modules() const;
    modules_holder_map_t &modules();

    std::optional<time::range> total_range() const;

    void insert_module(time::range, module);
    void erase_module(module const &);

    track copy() const;

    void process(time::range const &, stream &);

    chaining::chain_sync_t<event_t> chain();
};

track::modules_map_t copy_modules(track::modules_map_t const &);
}  // namespace yas::proc
