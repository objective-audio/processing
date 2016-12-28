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
    std::multimap<time_range, module> _modules;
    
    void process(stream &stream) {
        auto const &stream_time_range = stream.time_range();
        for (auto &pair : _modules) {
            if (pair.first.is_overlap(stream_time_range)) {
                pair.second.process(stream);
            }
        }
    }
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

void processing::track::insert_module(module module) {
    impl_ptr<impl>()->_modules.emplace(module.time_range(), std::move(module));
}

void processing::track::process(stream &stream) {
    impl_ptr<impl>()->process(stream);
}
