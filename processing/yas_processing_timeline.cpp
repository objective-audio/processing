//
//  yas_processing_timeline.cpp
//

#include "yas_processing_timeline.h"
#include "yas_processing_track.h"

using namespace yas;

#pragma mark - timeline::impl

struct processing::timeline::impl : base::impl {
    std::map<int64_t, processing::track> _tracks;
};

#pragma mark - timeline

processing::timeline::timeline() : base(std::make_shared<impl>()) {
}

processing::timeline::timeline(std::nullptr_t) : base(nullptr) {
}

std::map<int64_t, processing::track> const &processing::timeline::tracks() const {
    return impl_ptr<impl>()->_tracks;
}

std::map<int64_t, processing::track> &processing::timeline::tracks() {
    return impl_ptr<impl>()->_tracks;
}

void processing::timeline::insert_track(int64_t const idx, processing::track track) {
    impl_ptr<impl>()->_tracks.emplace(idx, std::move(track));
}

std::size_t processing::timeline::track_count() const {
    return impl_ptr<impl>()->_tracks.size();
}

bool processing::timeline::has_track(int64_t const idx) const {
    return impl_ptr<impl>()->_tracks.count(idx) > 0;
}

std::experimental::optional<processing::track> processing::timeline::track(int64_t const idx) const {
    if (has_track(idx)) {
        return impl_ptr<impl>()->_tracks.at(idx);
    } else {
        return std::experimental::nullopt;
    }
}