//
//  yas_processing_timeline.cpp
//

#include "yas_processing_timeline.h"
#include "yas_processing_stream.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_track.h"

using namespace yas;

#pragma mark - timeline

proc::timeline::timeline(track_map_t &&tracks)
    : _tracks_holder(tracks_holder_t::make_shared(std::move(tracks))),
      _tracks_sender(std::dynamic_pointer_cast<tracks_sender_t>(this->_tracks_holder)) {
}

proc::timeline::track_map_t const &proc::timeline::tracks() const {
    return this->_tracks_holder->raw();
}

bool proc::timeline::insert_track(track_index_t const trk_idx, proc::track_ptr const &track) {
    auto const &tracks_holder = this->_tracks_holder;
    if (!tracks_holder->has_value(trk_idx)) {
        tracks_holder->insert_or_replace(trk_idx, track);
        return true;
    } else {
        return false;
    }
}

void proc::timeline::erase_track(track_index_t const trk_idx) {
    this->_tracks_holder->erase_for_key(trk_idx);
}

std::size_t proc::timeline::track_count() const {
    return this->_tracks_holder->size();
}

bool proc::timeline::has_track(track_index_t const idx) const {
    return this->_tracks_holder->has_value(idx);
}

proc::track_ptr const &proc::timeline::track(track_index_t const trk_idx) const {
    return this->_tracks_holder->at(trk_idx);
}

std::optional<proc::time::range> proc::timeline::total_range() const {
    return proc::total_range(this->_tracks_holder->raw());
}

proc::timeline_ptr proc::timeline::copy() const {
    return timeline::make_shared(proc::copy_tracks(this->_tracks_holder->raw()));
}

void proc::timeline::process(time::range const &time_range, stream &stream) {
    for (auto &track_pair : this->_tracks_holder->raw()) {
        track_pair.second->process(time_range, stream);
    }
}

void proc::timeline::process(time::range const &range, sync_source const &sync_src, process_f const &handler) {
    this->_process_continuously(
        range, sync_src,
        [&handler](time::range const &range, stream const &stream, std::optional<track_index_t> const &trk_idx) {
            if (!trk_idx.has_value()) {
                return handler(range, stream);
            }
            return continuation::keep;
        });
}

void proc::timeline::process(time::range const &range, sync_source const &sync_src, process_track_f const &handler) {
    this->_process_continuously(range, sync_src, handler);
}

void proc::timeline::_process_continuously(time::range const &range, sync_source const &sync_src,
                                           process_track_f const &handler) {
    frame_index_t frame = range.frame;

    while (frame < range.next_frame()) {
        frame_index_t const sync_next_frame = frame + sync_src.slice_length;
        frame_index_t const &end_next_frame = range.next_frame();

        stream stream{sync_src};

        time::range const current_range = time::range{
            frame,
            static_cast<length_t>(sync_next_frame < end_next_frame ? sync_next_frame - frame : end_next_frame - frame)};

        if (this->_process_tracks(current_range, stream, handler) == continuation::abort) {
            break;
        }

        if (handler(current_range, stream, std::nullopt) == continuation::abort) {
            break;
        }

        frame += sync_src.slice_length;
    }
}

proc::continuation proc::timeline::_process_tracks(time::range const &current_range, stream &stream,
                                                   process_track_f const &handler) {
    for (auto &track_pair : this->_tracks_holder->raw()) {
        track_pair.second->process(current_range, stream);

        if (handler(current_range, stream, track_pair.first) == continuation::abort) {
            return continuation::abort;
        }
    }
    return continuation::keep;
}

chaining::chain_sync_t<proc::timeline::event_t> proc::timeline::chain() const {
    return this->_tracks_holder->chain();
}

chaining::chain_unsync_t<proc::timeline::event_t> proc::timeline::chain_unsync() const {
    return this->_tracks_sender->chain_unsync();
}

chaining::chain_sync_t<proc::timeline::event_t> proc::timeline::chain_sync() const {
    return this->_tracks_sender->chain_sync();
}

void proc::timeline::fetch_for(chaining::any_joint const &joint) const {
    this->_tracks_sender->fetch_for(joint);
}

void proc::timeline::broadcast(event_t const &value) const {
    this->_tracks_sender->broadcast(value);
}

void proc::timeline::erase_joint(std::uintptr_t const key) const {
    this->_tracks_sender->erase_joint(key);
}

void proc::timeline::send_value_to_target(event_t const &value, std::uintptr_t const key) const {
    this->_tracks_sender->send_value_to_target(value, key);
}

proc::timeline_ptr proc::timeline::make_shared() {
    return make_shared({});
}

proc::timeline_ptr proc::timeline::make_shared(track_map_t &&tracks) {
    return timeline_ptr(new timeline{std::move(tracks)});
}

proc::timeline::track_map_t proc::copy_tracks(timeline::track_map_t const &src_tracks) {
    std::map<track_index_t, proc::track_ptr> tracks;
    for (auto const &pair : src_tracks) {
        tracks.emplace(pair.first, pair.second->copy());
    }
    return tracks;
}
