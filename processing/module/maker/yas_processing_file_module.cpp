//
//  yas_processing_file_module.cpp
//

#include "yas_processing_file_module.h"

#include <audio/yas_audio_umbrella.h>
#include <processing/yas_processing_file_module_context.h>
#include <processing/yas_processing_module.h>
#include <processing/yas_processing_module_utils.h>
#include <processing/yas_processing_send_signal_processor.h>

#include <iostream>

using namespace yas;
using namespace yas::proc;

template <typename SampleType>
module_ptr file::make_signal_module(url const &url, frame_index_t const module_offset,
                                    frame_index_t const file_offset) {
    auto context = std::make_shared<file::context<SampleType>>(url, module_offset, file_offset);

    auto make_processors = [context] {
        auto prepare_processor = [context](time::range const &current_range, connector_map_t const &,
                                           connector_map_t const &,
                                           stream &) { std::cout << "yaso prepare" << std::endl; };

        auto send_processor = make_send_signal_processor<SampleType>(
            [context](time::range const &time_range, sync_source const &sync_src, channel_index_t const,
                      connector_index_t const co_idx, SampleType *const signal_ptr) {
                std::cout << "yaso read" << std::endl;
                context->read_from_file(time_range, sync_src, co_idx, signal_ptr);
            });

        return module::processors_t{{std::move(prepare_processor), std::move(send_processor)}};
    };

    return module::make_shared(std::move(make_processors));
}

template proc::module_ptr proc::file::make_signal_module<double>(url const &, frame_index_t const, frame_index_t const);
template proc::module_ptr proc::file::make_signal_module<float>(url const &, frame_index_t const, frame_index_t const);
template proc::module_ptr proc::file::make_signal_module<int32_t>(url const &, frame_index_t const,
                                                                  frame_index_t const);
template proc::module_ptr proc::file::make_signal_module<int16_t>(url const &, frame_index_t const,
                                                                  frame_index_t const);
