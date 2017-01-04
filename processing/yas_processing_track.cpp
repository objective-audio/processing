//
//  yas_processing_track.cpp
//

#include "yas_processing_track.h"
#include "yas_processing_time_range.h"
#include "yas_processing_module.h"
#include "yas_processing_stream.h"
#include "yas_stl_utils.h"

using namespace yas;

#pragma mark - processing::track::impl

struct processing::track::impl : base::impl {
    std::multimap<time::range, module> _modules;

    void process(time::range const &time_range, stream &stream) {
        for (auto &pair : _modules) {
            if (auto const current_time_range = pair.first.intersect(time_range)) {
                pair.second.process(*current_time_range, stream);
            }
        }
    }
};

#pragma mark - processing::track

processing::track::track() : base(std::make_shared<impl>()) {
}

processing::track::track(std::nullptr_t) : base(nullptr) {
}

std::multimap<processing::time::range, processing::module> const &processing::track::modules() const {
    return impl_ptr<impl>()->_modules;
}

std::multimap<processing::time::range, processing::module> &processing::track::modules() {
    return impl_ptr<impl>()->_modules;
}

void processing::track::insert_module(processing::time::range time_range, module module) {
    impl_ptr<impl>()->_modules.emplace(std::move(time_range), std::move(module));
}

void processing::track::process(time::range const &time_range, stream &stream) {
    impl_ptr<impl>()->process(time_range, stream);
}
