//
//  yas_processing_generator_modules.cpp
//

#include "yas_processing_generator_modules.h"
#include "yas_processing_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_signal_event.h"
#include "yas_fast_each.h"

using namespace yas;

template <typename T>
processing::module processing::make_signal_module(generator::kind const kind, frame_index_t const offset) {
    using namespace yas::processing::generator;

    auto prepare_processor = [](time::range const &, connector_map_t const &, connector_map_t const &,
                                stream &) mutable {};

    auto send_processor = processing::make_send_signal_processor<T>(
        [kind, offset](processing::time::range const &time_range, channel_index_t const,
                       connector_index_t const con_idx, T *const signal_ptr) {
            if (con_idx == to_connector_index(output_key::out)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                auto const top_idx = offset + time_range.frame;

                while (yas_fast_each_next(out_each)) {
                    auto const &idx = yas_fast_each_index(out_each);
                    switch (kind) {
                        case kind::second:
#warning todo
//                            yas_fast_each_value(out_each) = std::sin(phase_value);
                            break;
                    }
                }
            }
        });

    return processing::module{{std::move(prepare_processor), std::move(send_processor)}};
}

template processing::module processing::make_signal_module<double>(generator::kind const, frame_index_t const);
template processing::module processing::make_signal_module<float>(generator::kind const, frame_index_t const);
