//
//  yas_processing_track.h
//

#pragma once

#include <chaining/yas_chaining_umbrella.h>

#include <optional>

#include "yas_processing_module.h"
#include "yas_processing_time.h"

namespace yas::proc {
struct track final : chaining::sender<chaining::map::event> {
    using modules_map_t = std::map<time::range, std::vector<module_ptr>>;
    using modules_holder_map_t = std::map<time::range, module_vector_holder_ptr_t>;
    using event_type_t = chaining::event_type;
    using event_t = chaining::map::event;
    using fetched_event_t = chaining::map::fetched_event<time::range, module_vector_holder_ptr_t>;
    using inserted_event_t = chaining::map::inserted_event<time::range, module_vector_holder_ptr_t>;
    using erased_event_t = chaining::map::erased_event<time::range, module_vector_holder_ptr_t>;
    using relayed_event_t = chaining::map::relayed_event<time::range, module_vector_holder_ptr_t>;

    modules_holder_map_t const &modules() const;
    modules_holder_map_t &modules();

    std::optional<time::range> total_range() const;

    void push_back_module(module_ptr const &, time::range);
    void insert_module(module_ptr const &, std::size_t const, time::range);
    bool erase_module(module_ptr const &);
    bool erase_module(module_ptr const &, time::range const &);
    bool erase_module_at(std::size_t const, time::range const &);
    void erase_modules_for_range(time::range const &);

    track_ptr copy() const;

    void process(time::range const &, stream &);

    chaining::chain_sync_t<event_t> chain();

    chaining::chain_unsync_t<event_t> chain_unsync() const override;
    chaining::chain_sync_t<event_t> chain_sync() const override;

    static track_ptr make_shared();
    static track_ptr make_shared(modules_map_t &&);

   private:
    using modules_holder_t = chaining::map::holder<time::range, module_vector_holder_ptr_t>;
    using modules_holder_ptr_t = chaining::map::holder_ptr<time::range, module_vector_holder_ptr_t>;
    using modules_sender_t = chaining::sender_protocol<proc::track::event_t>;

    modules_holder_ptr_t const _modules_holder;
    std::shared_ptr<modules_sender_t> const _modules_sender;

    explicit track(modules_map_t &&);

    void fetch_for(chaining::any_joint const &joint) const override;
    void broadcast(event_t const &value) const override;
    void erase_joint(std::uintptr_t const key) const override;
    void send_value_to_target(event_t const &value, std::uintptr_t const key) const override;
};

std::optional<time::range> total_range(std::map<track_index_t, track_ptr> const &);
track::modules_map_t copy_modules(track::modules_map_t const &);
track::modules_map_t copy_to_modules(track::modules_holder_map_t const &);
track::modules_holder_map_t to_modules_holders(track::modules_map_t &&);
}  // namespace yas::proc
