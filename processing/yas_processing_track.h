//
//  yas_processing_track.h
//

#pragma once

#include <chaining/yas_chaining_umbrella.h>
#include <cpp_utils/yas_weakable.h>
#include <optional>
#include "yas_processing_module.h"
#include "yas_processing_time.h"

namespace yas::proc {
class module;
class stream;

struct track : chaining::sender<chaining::map::event>, weakable<track> {
    using modules_map_t = std::map<time::range, std::vector<module>>;
    using modules_holder_map_t = std::map<time::range, chaining::vector::holder<module>>;
    using event_type_t = chaining::event_type;
    using event_t = chaining::map::event;
    using fetched_event_t = chaining::map::fetched_event<time::range, chaining::vector::holder<module>>;
    using inserted_event_t = chaining::map::inserted_event<time::range, chaining::vector::holder<module>>;
    using erased_event_t = chaining::map::erased_event<time::range, chaining::vector::holder<module>>;
    using relayed_event_t = chaining::map::relayed_event<time::range, chaining::vector::holder<module>>;

    struct impl : chaining::sender<event_t>::impl, weakable_impl {
        chaining::map::holder<time::range, module_vector_holder_t> _modules_holder;

        impl();
        impl(modules_map_t &&modules);
        void push_back_module(module &&module, time::range &&range);
        void insert_module(module &&module, std::size_t const idx, time::range &&range);
        bool erase_module(module const &erasing);
        bool erase_module(module const &erasing, time::range const &range);
        bool erase_module_at(std::size_t const idx, time::range const &range);
        void erase_modules_for_range(time::range const &range);
        void process(time::range const &time_range, stream &stream);
        std::optional<time::range> total_range();
        track copy();
        void broadcast(event_t const &value) override;
        void send_value_to_target(event_t const &value, std::uintptr_t const key) override;
        void erase_joint(std::uintptr_t const key) override;
        void fetch_for(chaining::any_joint const &joint) override;
        chaining::chain_unsync_t<event_t> chain_unsync() override;
        chaining::chain_sync_t<event_t> chain_sync() override;
    };

    track();
    explicit track(modules_map_t &&);
    explicit track(std::shared_ptr<impl> &&);

    modules_holder_map_t const &modules() const;
    modules_holder_map_t &modules();

    std::optional<time::range> total_range() const;

    void push_back_module(module, time::range);
    void insert_module(module, std::size_t const, time::range);
    bool erase_module(module const &);
    bool erase_module(module const &, time::range const &);
    bool erase_module_at(std::size_t const, time::range const &);
    void erase_modules_for_range(time::range const &);

    track copy() const;

    void process(time::range const &, stream &);

    chaining::chain_sync_t<event_t> chain();

    std::shared_ptr<weakable_impl> weakable_impl_ptr() const override;
};

std::optional<time::range> total_range(std::map<track_index_t, track> const &);
track::modules_map_t copy_modules(track::modules_map_t const &);
track::modules_map_t copy_to_modules(track::modules_holder_map_t const &);
track::modules_holder_map_t to_modules_holders(track::modules_map_t &&);
}  // namespace yas::proc
