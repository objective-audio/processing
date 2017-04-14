//
//  yas_processing_generator_modules.cpp
//

#include "yas_processing_generator_modules.h"
#include "yas_processing_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_sync_source.h"
#include "yas_fast_each.h"

using namespace yas;

template <typename T>
processing::module processing::make_signal_module(generator::kind const kind, frame_index_t const offset) {
    using namespace yas::processing::generator;

    auto prepare_processor = [](time::range const &, connector_map_t const &, connector_map_t const &,
                                stream &) mutable {};

    auto send_processor = processing::make_send_signal_processor<T>(
        [kind, offset](processing::time::range const &time_range, sync_source const &sync_src, channel_index_t const,
                       connector_index_t const co_idx, T *const signal_ptr) {
            if (co_idx == to_connector_index(output::value)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                auto const top_idx = offset + time_range.frame;
                T const sr = sync_src.sample_rate;

                while (yas_each_next(out_each)) {
                    auto const &idx = yas_each_index(out_each);
                    switch (kind) {
                        case kind::second:
                            yas_each_value(out_each) = (T)(top_idx + idx) / sr;
                            break;
                    }
                }
            }
        });

    return processing::module{{std::move(prepare_processor), std::move(send_processor)}};
}

template processing::module processing::make_signal_module<double>(generator::kind const, frame_index_t const);
template processing::module processing::make_signal_module<float>(generator::kind const, frame_index_t const);

#pragma mark -

void yas::connect(processing::module &module, processing::generator::output const &output,
                  processing::channel_index_t const &ch_idx) {
    module.connect_output(processing::to_connector_index(output), ch_idx);
}

std::string yas::to_string(processing::generator::output const &output) {
    using namespace yas::processing::generator;

    switch (output) {
        case output::value:
            return "value";
    }

    throw "output not found.";
}
