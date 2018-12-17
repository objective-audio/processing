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
    std::multimap<time::range, module> _modules;

    void process(time::range const &time_range, stream &stream) {
        for (auto &pair : this->_modules) {
            if (auto const current_time_range = pair.first.intersect(time_range)) {
                pair.second.process(*current_time_range, stream);
            }
        }
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

std::multimap<proc::time::range, proc::module> &proc::track::modules() {
    return this->impl_ptr<impl>()->_modules;
}

void proc::track::insert_module(proc::time::range time_range, module module) {
    this->impl_ptr<impl>()->_modules.emplace(std::move(time_range), std::move(module));
}

void proc::track::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}
