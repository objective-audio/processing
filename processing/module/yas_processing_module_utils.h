//
//  yas_processing_module_utils.h
//

#pragma once

#include <audio/yas_audio_umbrella.h>
#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_time.h>

namespace yas::proc {
template <typename T>
audio::pcm_format pcm_format();

// offsetはmoduleの置いてある位置。module内のframeを返す
frame_index_t module_frame(frame_index_t const &time_frame, frame_index_t const offset);

struct module_file_range_result {
    time::range range;
    frame_index_t offset;
};

std::optional<module_file_range_result> module_file_range(time::range const &, frame_index_t const module_offset,
                                                          frame_index_t const file_offset,
                                                          frame_index_t const file_length);
}  // namespace yas::proc
