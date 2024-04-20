//
//  yas_processing_file_module_context.h
//

#pragma once

#include <processing/common/yas_processing_common_types.h>
#include <processing/common/yas_processing_ptr.h>
#include <processing/sync_source/yas_processing_sync_source.h>
#include <processing/time/yas_processing_time.h>

#include <filesystem>

namespace yas::proc::file {
template <typename SampleType>
struct context {
    context(std::filesystem::path const &, frame_index_t const module_offset, frame_index_t const file_offset);

    void read_from_file(time::range const &time_range, sync_source const &sync_src, connector_index_t const co_idx,
                        SampleType *const signal_ptr) const;

   private:
    std::filesystem::path _path;
    frame_index_t const _module_offset;
    frame_index_t const _file_offset;
};
}  // namespace yas::proc::file
