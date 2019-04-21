//
//  yas_processing_track.cpp
//

#include "yas_processing_track.h"
#include <cpp_utils/yas_stl_utils.h>
#include "yas_processing_module.h"
#include "yas_processing_stream.h"

using namespace yas;

#pragma mark - proc::track::impl

struct proc::track::impl : chaining::sender<event_t>::impl {
    chaining::map::holder<time::range, module_vector_holder_t> _modules_holder;

    impl() {
    }

    impl(modules_map_t &&modules) : _modules_holder(to_modules_holders(std::move(modules))) {
    }

    void insert_module(time::range &&range, module &&module) {
        if (this->_modules_holder.has_value(range)) {
            this->_modules_holder.at(range).push_back(std::move(module));
        } else {
            this->_modules_holder.insert_or_replace(std::move(range),
                                                    chaining::vector::holder<proc::module>{{std::move(module)}});
        }
    }

    bool erase_module(module const &erasing) {
        for (auto &pair : this->_modules_holder.raw()) {
            if (this->erase_module(erasing, pair.first)) {
                return true;
            }
        }
        return false;
    }

    bool erase_module(module const &erasing, time::range const &range) {
        if (this->_modules_holder.has_value(range)) {
            auto &modules = this->_modules_holder.at(range);

            std::size_t idx = 0;
            for (auto const &module : modules.raw()) {
                if (module == erasing) {
                    if (modules.size() == 1) {
                        this->_modules_holder.erase_for_key(range);
                    } else {
                        modules.erase_at(idx);
                    }
                    return true;
                }
                ++idx;
            }
        }
        return false;
    }

    void erase_modules_for_range(time::range const &range) {
        this->_modules_holder.erase_for_key(range);
    }

    void process(time::range const &time_range, stream &stream) {
        for (auto &pair : this->_modules_holder.raw()) {
            if (auto const current_time_range = pair.first.intersected(time_range)) {
                for (auto &module : pair.second.raw()) {
                    module.process(*current_time_range, stream);
                }
            }
        }
    }

    std::optional<time::range> total_range() {
        std::optional<time::range> result{std::nullopt};

        for (auto const &pair : this->_modules_holder.raw()) {
            if (result) {
                result = result->merged(pair.first);
            } else {
                result = pair.first;
            }
        }

        return result;
    }

    track copy() {
        return proc::track{copy_to_modules(this->_modules_holder.raw())};
    }

    void broadcast(event_t const &value) override {
        this->_modules_holder.sendable().broadcast(value);
    }

    void send_value_to_target(event_t const &value, std::uintptr_t const key) override {
        this->_modules_holder.sendable().send_value_to_target(value, key);
    }

    void erase_joint(std::uintptr_t const key) override {
        this->_modules_holder.sendable().erase_joint(key);
    }

    void fetch_for(chaining::any_joint const &joint) override {
        this->_modules_holder.sendable().fetch_for(joint);
    }

    chaining::chain_unsync_t<event_t> chain_unsync() override {
        return this->_modules_holder.sendable().chain_unsync();
    }

    chaining::chain_sync_t<event_t> chain_sync() override {
        return this->_modules_holder.sendable().chain_sync();
    }
};

#pragma mark - proc::track

proc::track::track() : chaining::sender<event_t>(std::make_shared<impl>()) {
}

proc::track::track(modules_map_t &&modules) : chaining::sender<event_t>(std::make_shared<impl>(std::move(modules))) {
}

proc::track::track(std::nullptr_t) : chaining::sender<event_t>(nullptr) {
}

proc::track::modules_holder_map_t const &proc::track::modules() const {
    return this->impl_ptr<impl>()->_modules_holder.raw();
}

proc::track::modules_holder_map_t &proc::track::modules() {
    return this->impl_ptr<impl>()->_modules_holder.raw();
}

std::optional<proc::time::range> proc::track::total_range() const {
    return impl_ptr<impl>()->total_range();
}

void proc::track::insert_module(proc::time::range time_range, module module) {
    this->impl_ptr<impl>()->insert_module(std::move(time_range), std::move(module));
}

bool proc::track::erase_module(module const &module) {
    return this->impl_ptr<impl>()->erase_module(module);
}

bool proc::track::erase_module(module const &module, time::range const &range) {
    return this->impl_ptr<impl>()->erase_module(module, range);
}

void proc::track::erase_modules_for_range(time::range const &range) {
    this->impl_ptr<impl>()->erase_modules_for_range(range);
}

proc::track proc::track::copy() const {
    return this->impl_ptr<impl>()->copy();
}

void proc::track::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}

chaining::chain_sync_t<proc::track::event_t> proc::track::chain() {
    return this->impl_ptr<impl>()->chain_sync();
}

proc::track::modules_map_t proc::copy_modules(track::modules_map_t const &src_modules) {
    track::modules_map_t result;
    for (auto const &pair : src_modules) {
        result.emplace(pair.first, copy(pair.second));
    }
    return result;
}

proc::track::modules_map_t proc::copy_to_modules(track::modules_holder_map_t const &modules) {
    track::modules_map_t map;
    for (auto const &pair : modules) {
        module_vector_t copied;
        for (auto const &module : pair.second.raw()) {
            copied.emplace_back(module.copy());
        }
        map.emplace(pair.first, std::move(copied));
    }
    return map;
}

proc::track::modules_holder_map_t proc::to_modules_holders(track::modules_map_t &&modules) {
    track::modules_holder_map_t map;
    for (auto &pair : modules) {
        map.emplace(pair.first, std::move(pair.second));
    }
    return map;
}
