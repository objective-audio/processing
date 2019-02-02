//
//  yas_processing_generator_modules.cpp
//

#include "yas_processing_generator_modules.h"
#include <cpp_utils/yas_fast_each.h>
#include "yas_processing_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_sync_source.h"

using namespace yas;

template <typename T>
proc::module proc::make_signal_module(generator::kind const kind, frame_index_t const offset) {
    using namespace yas::proc::generator;

    auto make_processors = [kind, offset] {
        auto prepare_processor = [](time::range const &, connector_map_t const &, connector_map_t const &,
                                    stream &) mutable {};

        auto send_processor = proc::make_send_signal_processor<T>(
            [kind, offset, out_each = fast_each<T *>{}](proc::time::range const &time_range,
                                                        sync_source const &sync_src, channel_index_t const,
                                                        connector_index_t const co_idx, T *const signal_ptr) mutable {
                if (co_idx == to_connector_index(output::value)) {
                    out_each.reset(signal_ptr, time_range.length);
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

        return module::processors_t{{std::move(prepare_processor), std::move(send_processor)}};
    };

    return proc::module{std::move(make_processors)};
}

template proc::module proc::make_signal_module<double>(generator::kind const, frame_index_t const);
template proc::module proc::make_signal_module<float>(generator::kind const, frame_index_t const);

#pragma mark -

void yas::connect(proc::module &module, proc::generator::output const &output, proc::channel_index_t const &ch_idx) {
    module.connect_output(proc::to_connector_index(output), ch_idx);
}

std::string yas::to_string(proc::generator::output const &output) {
    using namespace yas::proc::generator;

    switch (output) {
        case output::value:
            return "value";
    }

    throw "output not found.";
}
