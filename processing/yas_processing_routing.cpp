//
//  yas_processing_routing.cpp
//

#include "yas_processing_routing.h"
#include "yas_processing_track.h"

using namespace yas;

#pragma mark - routing::impl

struct processing::routing::impl : base::impl {
    std::map<int64_t, processing::track> _tracks;
};

#pragma mark - routing

processing::routing::routing() : base(std::make_shared<impl>()) {
}

processing::routing::routing(std::nullptr_t) : base(nullptr) {
}

std::map<int64_t, processing::track> const &processing::routing::tracks() const {
    return impl_ptr<impl>()->_tracks;
}

std::map<int64_t, processing::track> &processing::routing::tracks() {
    return impl_ptr<impl>()->_tracks;
}

void processing::routing::insert_track(int64_t const idx, processing::track track) {
    impl_ptr<impl>()->_tracks.emplace(idx, std::move(track));
}

std::size_t processing::routing::track_count() const {
    return impl_ptr<impl>()->_tracks.size();
}

bool processing::routing::has_track(int64_t const idx) const {
    return impl_ptr<impl>()->_tracks.count(idx) > 0;
}

std::experimental::optional<processing::track> processing::routing::track(int64_t const idx) const {
    if (has_track(idx)) {
        return impl_ptr<impl>()->_tracks.at(idx);
    } else {
        return std::experimental::nullopt;
    }
}

