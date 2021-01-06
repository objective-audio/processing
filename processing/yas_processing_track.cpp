//
//  yas_processing_track.cpp
//

#include "yas_processing_track.h"

#include <cpp_utils/yas_stl_utils.h>

#include "yas_processing_stream.h"

using namespace yas;

proc::track::track(modules_map_t &&modules)
    : _modules_holder(modules_holder_t::make_shared(to_modules_holders(std::move(modules)))),
      _modules_sender(std::dynamic_pointer_cast<modules_sender_t>(this->_modules_holder)) {
}

proc::track::modules_holder_map_t const &proc::track::modules() const {
    return this->_modules_holder->raw();
}

proc::track::modules_holder_map_t &proc::track::modules() {
    return this->_modules_holder->raw();
}

std::optional<proc::time::range> proc::track::total_range() const {
    std::optional<time::range> result{std::nullopt};

    for (auto const &pair : this->_modules_holder->raw()) {
        if (result) {
            result = result->merged(pair.first);
        } else {
            result = pair.first;
        }
    }

    return result;
}

void proc::track::push_back_module(module_ptr const &module, proc::time::range range) {
    if (this->_modules_holder->has_value(range)) {
        this->_modules_holder->at(range)->push_back(module);
    } else {
        this->_modules_holder->insert_or_replace(std::move(range), module_vector_holder_t::make_shared({module}));
    }
}

void proc::track::insert_module(module_ptr const &module, module_index_t const idx, time::range range) {
    if (this->_modules_holder->has_value(range) && idx <= this->_modules_holder->at(range)->size()) {
        this->_modules_holder->at(range)->insert(std::move(module), idx);
    } else if (idx == 0) {
        this->_modules_holder->insert_or_replace(std::move(range),
                                                 module_vector_holder_t::make_shared({std::move(module)}));
    } else {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + " : out of range. index(" + std::to_string(idx) +
                                ")");
    }
}

bool proc::track::erase_module(module_ptr const &module) {
    for (auto &pair : this->_modules_holder->raw()) {
        if (this->erase_module(module, pair.first)) {
            return true;
        }
    }
    return false;
}

bool proc::track::erase_module(module_ptr const &erasing, time::range const &range) {
    if (this->_modules_holder->has_value(range)) {
        auto const &modules = this->_modules_holder->at(range);

        std::size_t idx = 0;
        for (auto const &module : modules->raw()) {
            if (module == erasing) {
                if (modules->size() == 1) {
                    this->_modules_holder->erase_for_key(range);
                } else {
                    modules->erase_at(idx);
                }
                return true;
            }
            ++idx;
        }
    }
    return false;
}

bool proc::track::erase_module_at(module_index_t const idx, time::range const &range) {
    if (this->_modules_holder->has_value(range)) {
        auto &modules = this->_modules_holder->at(range);
        if (idx < modules->size()) {
            this->_modules_holder->at(range)->erase_at(idx);
            return true;
        }
    }
    return false;
}

void proc::track::erase_modules_for_range(time::range const &range) {
    this->_modules_holder->erase_for_key(range);
}

proc::track_ptr proc::track::copy() const {
    return proc::track::make_shared(copy_to_modules(this->_modules_holder->raw()));
}

void proc::track::process(time::range const &time_range, stream &stream) {
    for (auto const &pair : this->_modules_holder->raw()) {
        if (auto const current_time_range = pair.first.intersected(time_range)) {
            for (auto &module : pair.second->raw()) {
                module->process(*current_time_range, stream);
            }
        }
    }
}

chaining::chain_sync_t<proc::track::event_t> proc::track::chain() {
    return this->chain_sync();
}

chaining::chain_unsync_t<proc::track::event_t> proc::track::chain_unsync() const {
    return this->_modules_sender->chain_unsync();
}

chaining::chain_sync_t<proc::track::event_t> proc::track::chain_sync() const {
    return this->_modules_sender->chain_sync();
}

void proc::track::fetch_for(chaining::any_joint const &joint) const {
    this->_modules_sender->fetch_for(joint);
}

void proc::track::broadcast(event_t const &value) const {
    this->_modules_sender->broadcast(value);
}

void proc::track::erase_joint(std::uintptr_t const key) const {
    this->_modules_sender->erase_joint(key);
}

void proc::track::send_value_to_target(event_t const &value, std::uintptr_t const key) const {
    this->_modules_sender->send_value_to_target(value, key);
}

proc::track_ptr proc::track::make_shared() {
    return make_shared({});
}

proc::track_ptr proc::track::make_shared(modules_map_t &&modules) {
    return track_ptr(new track{std::move(modules)});
}

#pragma mark - utils

std::optional<proc::time::range> proc::total_range(std::map<track_index_t, track_ptr> const &tracks) {
    std::optional<proc::time::range> result{std::nullopt};

    for (auto &track_pair : tracks) {
        if (auto const &track_range = track_pair.second->total_range()) {
            if (result) {
                result = result->merged(*track_range);
            } else {
                result = track_range;
            }
        }
    }

    return result;
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
        for (auto const &module : pair.second->raw()) {
            copied.emplace_back(module->copy());
        }
        map.emplace(pair.first, std::move(copied));
    }
    return map;
}

proc::track::modules_holder_map_t proc::to_modules_holders(track::modules_map_t &&modules) {
    track::modules_holder_map_t map;
    for (auto &pair : modules) {
        map.emplace(pair.first, module_vector_holder_t::make_shared(std::move(pair.second)));
    }
    return map;
}
