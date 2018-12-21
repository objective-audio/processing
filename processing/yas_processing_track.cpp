//
//  yas_processing_track.cpp
//

#include "yas_processing_track.h"
#include "yas_processing_module.h"
#include "yas_processing_stream.h"
#include "yas_stl_utils.h"

using namespace yas;

#pragma mark - proc::track::impl

struct proc::track::impl : base::impl {
    modules_map_t _modules;

    void insert_module(time::range &&range, module &&module) {
        this->_modules.emplace(std::move(range), std::move(module));
    }

    void remove_module(module const &module) {
        erase_if(this->_modules,
                 [&module](std::pair<time::range, proc::module> const &pair) { return pair.second == module; });
    }

    void process(time::range const &time_range, stream &stream) {
        for (auto &pair : this->_modules) {
            if (auto const current_time_range = pair.first.intersected(time_range)) {
                pair.second.process(*current_time_range, stream);
            }
        }
    }

    std::optional<time::range> total_range() {
        std::optional<time::range> result{std::nullopt};

        for (auto const &pair : this->_modules) {
            if (result) {
                result = result->merged(pair.first);
            } else {
                result = pair.first;
            }
        }

        return result;
    }
};

#pragma mark - proc::track

proc::track::track() : base(std::make_shared<impl>()) {
}

proc::track::track(std::nullptr_t) : base(nullptr) {
}

std::multimap<proc::time::range, proc::module> const &proc::track::modules() const {
    return this->impl_ptr<impl>()->_modules;
}

proc::track::modules_map_t &proc::track::modules() {
    return this->impl_ptr<impl>()->_modules;
}

std::optional<proc::time::range> proc::track::total_range() const {
    return impl_ptr<impl>()->total_range();
}

void proc::track::insert_module(proc::time::range time_range, module module) {
    this->impl_ptr<impl>()->insert_module(std::move(time_range), std::move(module));
}

void proc::track::remove_module(module const &module) {
    this->impl_ptr<impl>()->remove_module(module);
}

void proc::track::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}
