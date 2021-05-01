//
//  yas_processing_file_module_context.h
//

#pragma once

#include <cpp_utils/yas_url.h>
#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_ptr.h>
#include <processing/yas_processing_sync_source.h>
#include <processing/yas_processing_time.h>

namespace yas::proc::file {
template <typename SampleType>
struct context {
    context(url const &url, frame_index_t const module_offset, frame_index_t const file_offset);

    void read_from_file(time::range const &time_range, sync_source const &sync_src, connector_index_t const co_idx,
                        SampleType *const signal_ptr) const;

   private:
    url _url;
    frame_index_t const _module_offset;
    frame_index_t const _file_offset;
};
}  // namespace yas::proc::file
