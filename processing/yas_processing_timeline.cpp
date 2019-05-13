//
//  yas_processing_timeline.cpp
//

#include "yas_processing_timeline.h"
#include "yas_processing_stream.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_track.h"

using namespace yas;

#pragma mark - timeline::impl

struct proc::timeline::impl : chaining::sender<event_t>::impl {
    chaining::map::holder<track_index_t, proc::track> _tracks_holder;

    impl() {
    }

    impl(track_map_t &&tracks) : _tracks_holder(std::move(tracks)) {
    }

    std::optional<proc::time::range> total_range() {
        return proc::total_range(this->_tracks_holder.raw());
    }

    timeline copy() {
        return timeline{proc::copy_tracks(this->_tracks_holder.raw())};
    }

    void broadcast(event_t const &value) override {
        this->_tracks_holder.sendable().broadcast(value);
    }

    void send_value_to_target(event_t const &value, std::uintptr_t const key) override {
        this->_tracks_holder.sendable().send_value_to_target(value, key);
    }

    void erase_joint(std::uintptr_t const key) override {
        this->_tracks_holder.sendable().erase_joint(key);
    }

    void fetch_for(chaining::any_joint const &joint) override {
        this->_tracks_holder.sendable().fetch_for(joint);
    }

    chaining::chain_unsync_t<event_t> chain_unsync() override {
        return this->_tracks_holder.sendable().chain_unsync();
    }

    chaining::chain_sync_t<event_t> chain_sync() override {
        return this->_tracks_holder.sendable().chain_sync();
    }

    void process(time::range const &time_range, stream &stream) {
        for (auto &track_pair : this->_tracks_holder.raw()) {
            track_pair.second.process(time_range, stream);
        }
    }

    void process_continuously(time::range const &range, sync_source const &sync_src, process_track_f const &handler) {
        frame_index_t frame = range.frame;

        while (frame < range.next_frame()) {
            frame_index_t const sync_next_frame = frame + sync_src.slice_length;
            frame_index_t const &end_next_frame = range.next_frame();

            stream stream{sync_src};

            time::range const current_range =
                time::range{frame, static_cast<length_t>(sync_next_frame < end_next_frame ? sync_next_frame - frame :
                                                                                            end_next_frame - frame)};

            if (this->_process_tracks(current_range, stream, handler) == continuation::abort) {
                break;
            }

            if (handler(current_range, stream, std::nullopt) == continuation::abort) {
                break;
            }

            frame += sync_src.slice_length;
        }
    }

   private:
    continuation _process_tracks(time::range const &current_range, stream &stream, process_track_f const &handler) {
        for (auto &track_pair : this->_tracks_holder.raw()) {
            track_pair.second.process(current_range, stream);

            if (handler(current_range, stream, track_pair.first) == continuation::abort) {
                return continuation::abort;
            }
        }
        return continuation::keep;
    }
};

#pragma mark - timeline

proc::timeline::timeline() : chaining::sender<event_t>(std::make_shared<impl>()) {
}

proc::timeline::timeline(track_map_t &&tracks) : chaining::sender<event_t>(std::make_shared<impl>(std::move(tracks))) {
}

proc::timeline::timeline(std::nullptr_t) : chaining::sender<event_t>(nullptr) {
}

proc::timeline::track_map_t const &proc::timeline::tracks() const {
    return this->impl_ptr<impl>()->_tracks_holder.raw();
}

proc::timeline::track_map_t &proc::timeline::tracks() {
    return this->impl_ptr<impl>()->_tracks_holder.raw();
}

bool proc::timeline::insert_track(track_index_t const trk_idx, proc::track track) {
    auto &tracks_holder = this->impl_ptr<impl>()->_tracks_holder;
    if (!tracks_holder.has_value(trk_idx)) {
        tracks_holder.insert_or_replace(trk_idx, std::move(track));
        return true;
    } else {
        return false;
    }
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

proc::timeline proc::timeline::copy() const {
    return impl_ptr<impl>()->copy();
}

void proc::timeline::process(time::range const &time_range, stream &stream) {
    this->impl_ptr<impl>()->process(time_range, stream);
}

void proc::timeline::process(time::range const &range, sync_source const &sync_src, process_f const handler) {
    this->impl_ptr<impl>()->process_continuously(
        range, sync_src,
        [&handler](time::range const &range, stream const &stream, std::optional<track_index_t> const &trk_idx) {
            if (!trk_idx.has_value()) {
                return handler(range, stream);
            }
            return continuation::keep;
        });
}

void proc::timeline::process(time::range const &range, sync_source const &sync_src, process_track_f const handler) {
    this->impl_ptr<impl>()->process_continuously(range, sync_src, handler);
}

chaining::chain_sync_t<proc::timeline::event_t> proc::timeline::chain() const {
    return this->impl_ptr<impl>()->_tracks_holder.chain();
}

proc::timeline::track_map_t proc::copy_tracks(timeline::track_map_t const &src_tracks) {
    std::map<track_index_t, proc::track> tracks;
    for (auto const &pair : src_tracks) {
        tracks.emplace(pair.first, pair.second.copy());
    }
    return tracks;
}
