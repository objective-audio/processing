//
//  yas_processing_track.cpp
//

#include "yas_processing_track.h"
#include "yas_processing_time_range.h"
#include "yas_processing_module.h"

using namespace yas;

#pragma mark - processing::track::impl

struct processing::track::impl : base::impl {
    std::multimap<time_range, module> _modules;
};

#pragma mark - processing::track

processing::track::track() : base(std::make_shared<impl>()) {
}

processing::track::track(std::nullptr_t) : base(nullptr) {
}

std::multimap<processing::time_range, processing::module> const &processing::track::modules() const {
    return impl_ptr<impl>()->_modules;
}

std::multimap<processing::time_range, processing::module> &processing::track::modules() {
    return impl_ptr<impl>()->_modules;
}

void processing::track::insert_module(time_range time_range, module module) {
    impl_ptr<impl>()->_modules.emplace(std::move(time_range), std::move(module));
}
