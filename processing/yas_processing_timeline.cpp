//
//  yas_processing_timeline.cpp
//

#include "yas_processing_timeline.h"
#include "yas_processing_stream.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_track.h"

using namespace yas;

#pragma mark - timeline::impl

struct proc::timeline::impl : chaining::sender<timeline_event_t>::impl {
    chaining::map::holder<track_index_t, proc::track> _tracks_holder;

    void process(time::range const &time_range, stream &stream) {
        for (auto &track_pair : this->_tracks_holder.raw()) {
            track_pair.second.process(time_range, stream);
        }
    }

    std::optional<proc::time::range> total_range() {
        std::optional<proc::time::range> result{std::nullopt};

        for (auto &track_pair : this->_tracks_holder.raw()) {
            if (auto const &track_range = track_pair.second.total_range()) {
                if (result) {
                    result = result->merged(*track_range);
                } else {
                    result = track_range;
                }
            }
        }

        return result;
    }

    void broadcast(timeline_event_t const &value) override {
        this->_tracks_holder.sendable().broadcast(value);
    }

    void send_value_to_target(timeline_event_t const &value, std::uintptr_t const key) override {
        this->_tracks_holder.sendable().send_value_to_target(value, key);
    }

    void erase_joint(std::uintptr_t const key) override {
        this->_tracks_holder.sendable().erase_joint(key);
    }

    void fetch_for(chaining::any_joint const &joint) override {
        this->_tracks_holder.sendable().fetch_for(joint);
    }

    chaining::chain_unsync_t<timeline_event_t> chain_unsync() override {
        return this->_tracks_holder.sendable().chain_unsync();
    }

    chaining::chain_sync_t<timeline_event_t> chain_sync() override {
        return this->_tracks_holder.sendable().chain_sync();
    }
};

#pragma mark - timeline

proc::timeline::timeline() : chaining::sender<timeline_event_t>(std::make_shared<impl>()) {
}

proc::timeline::timeline(std::nullptr_t) : chaining::sender<timeline_event_t>(nullptr) {
}

proc::timeline::track_map_t const &proc::timeline::tracks() const {
    return this->impl_ptr<impl>()->_tracks_holder.raw();
}

proc::timeline::track_map_t &proc::timeline::tracks() {
    return this->impl_ptr<impl>()->_tracks_holder.raw();
}

proc::track &proc::timeline::insert_track(track_index_t const trk_idx) {
    auto &tracks_holder = this->impl_ptr<impl>()->_tracks_holder;
    auto &tracks = tracks_holder.raw();
    if (tracks.count(trk_idx) == 0) {
        tracks_holder.insert_or_replace(trk_idx, proc::track{});
    }
    return tracks.at(trk_idx);
}

void proc::timeline::erase_track(track_index_t const trk_idx) {
    this->impl_ptr<impl>()->_tracks_holder.erase_for_key(trk_idx);
}

std::size_t proc::timeline::track_count() const {
    return this->impl_ptr<impl>()->_tracks_holder.size();
}

bool proc::timeline::has_track(track_index_t const idx) const {
    return this->impl_ptr<impl>()->_tracks_holder.has_value(idx);
}

proc::track const &proc::timeline::track(track_index_t const trk_idx) const {
    return this->impl_ptr<impl>()->_tracks_holder.at(trk_idx);
}

proc::track &proc::timeline::track(track_index_t const trk_idx) {
    return this->impl_ptr<impl>()->_tracks_holder.at(trk_idx);
}

std::optional<proc::time::range> proc::timeline::total_range() const {
    return impl_ptr<impl>()->total_range();
}

void proc::timeline::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}

void proc::timeline::process(time::range const &time_range, sync_source const &sync_src, offline_process_f handler) {
    frame_index_t frame = time_range.frame;

    while (frame < time_range.next_frame()) {
        frame_index_t const sync_next_frame = frame + sync_src.slice_length;
        frame_index_t const &end_next_frame = time_range.next_frame();

        stream stream{sync_src};

        time::range const current_time_range = time::range{
            frame,
            static_cast<length_t>(sync_next_frame < end_next_frame ? sync_next_frame - frame : end_next_frame - frame)};

        this->process(current_time_range, stream);

        bool stop = false;

        handler(current_time_range, stream, stop);

        if (stop) {
            break;
        }

        frame += sync_src.slice_length;
    }
}

chaining::chain_sync_t<proc::timeline_event_t> proc::timeline::chain() {
    return this->impl_ptr<impl>()->_tracks_holder.chain();
}
