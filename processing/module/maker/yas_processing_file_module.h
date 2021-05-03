//
//  yas_processing_file_module.h
//

#pragma once

#include <processing/yas_processing_file_module_context.h>

namespace yas::proc {
namespace file {
    template <typename SampleType>
    [[nodiscard]] module_ptr make_signal_module(url const &, frame_index_t const module_offset,
                                                frame_index_t const file_offset);
}  // namespace file
}  // namespace yas::proc
