//
//  yas_processing_timeline.cpp
//

#include "yas_processing_timeline.h"
#include "yas_processing_track.h"
#include "yas_processing_stream.h"
#include "yas_processing_sync_source.h"

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

processing::track &processing::timeline::add_track(track_index_t const trk_idx) {
    auto &tracks = this->impl_ptr<impl>()->_tracks;
    if (tracks.count(trk_idx) == 0) {
        tracks.emplace(trk_idx, processing::track{});
    }
    return tracks.at(trk_idx);
}

void processing::timeline::remove_track(track_index_t const trk_idx) {
    auto &tracks = this->impl_ptr<impl>()->_tracks;
    if (tracks.count(trk_idx) > 0) {
        tracks.erase(trk_idx);
    }
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

void processing::timeline::process(time::range const &time_range, sync_source const &sync_src,
                                   offline_process_f handler) {
    frame_index_t frame = time_range.frame;

    while (frame < time_range.next_frame()) {
        frame_index_t const sync_next_frame = frame + sync_src.slice_length;
        frame_index_t const &end_next_frame = time_range.next_frame();

        stream stream{sync_src};

        time::range const current_time_range = time::range{
            frame,
            static_cast<length_t>(sync_next_frame < end_next_frame ? sync_next_frame - frame : end_next_frame - frame)};

        this->process(current_time_range, stream);

        handler(current_time_range, stream);

        frame += sync_src.slice_length;
    }
}
