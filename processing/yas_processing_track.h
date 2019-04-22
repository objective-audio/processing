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

    void push_back_module(module, time::range);
    void insert_module(module, std::size_t const, time::range);
    bool erase_module(module const &);
    bool erase_module(module const &, time::range const &);
    void erase_modules_for_range(time::range const &);

    track copy() const;

    void process(time::range const &, stream &);

    chaining::chain_sync_t<event_t> chain();
};

track::modules_map_t copy_modules(track::modules_map_t const &);
track::modules_map_t copy_to_modules(track::modules_holder_map_t const &);
track::modules_holder_map_t to_modules_holders(track::modules_map_t &&);
}  // namespace yas::proc
