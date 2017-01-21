//
//  yas_processing_timeline.cpp
//

#include "yas_processing_timeline.h"
#include "yas_processing_track.h"

using namespace yas;

#pragma mark - timeline::impl

struct processing::timeline::impl : base::impl {
    track_map_t _tracks;

    void process(time::range const &time_range, stream &stream) {
        for (auto &track_pair : this->_tracks) {
            track_pair.second.process(time_range, stream);
        }
    }
};

#pragma mark - timeline

processing::timeline::timeline() : base(std::make_shared<impl>()) {
}

processing::timeline::timeline(std::nullptr_t) : base(nullptr) {
}

processing::timeline::track_map_t const &processing::timeline::tracks() const {
    return this->impl_ptr<impl>()->_tracks;
}

processing::timeline::track_map_t &processing::timeline::tracks() {
    return this->impl_ptr<impl>()->_tracks;
}

void processing::timeline::insert_track(track_index_t const idx, processing::track track) {
    this->impl_ptr<impl>()->_tracks.emplace(idx, std::move(track));
}

std::size_t processing::timeline::track_count() const {
    return this->impl_ptr<impl>()->_tracks.size();
}

bool processing::timeline::has_track(track_index_t const idx) const {
    return this->impl_ptr<impl>()->_tracks.count(idx) > 0;
}

processing::track const &processing::timeline::track(track_index_t const trk_idx) const {
    return this->impl_ptr<impl>()->_tracks.at(trk_idx);
}

processing::track &processing::timeline::track(track_index_t const trk_idx) {
    return this->impl_ptr<impl>()->_tracks.at(trk_idx);
}

void processing::timeline::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}
