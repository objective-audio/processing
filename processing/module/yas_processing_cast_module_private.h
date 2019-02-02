
//
//  yas_processing_cast_module_private.h
//

#pragma once

#include <cpp_utils/yas_fast_each.h>
#include "yas_processing_channel.h"
#include "yas_processing_module.h"
#include "yas_processing_number_event.h"
#include "yas_processing_number_process_context.h"
#include "yas_processing_receive_number_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_remove_number_processor.h"
#include "yas_processing_remove_signal_processor.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_signal_process_context.h"

namespace yas::proc::cast {
template <typename In, typename Out>
proc::module make_signal_module() {
    auto make_processors = [] {
        auto context = std::make_shared<signal_process_context<In, 1>>();

        auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                           stream &stream) mutable {
            context->reset(stream.sync_source().slice_length);
        };

        auto receive_processor = proc::make_receive_signal_processor<In>(
            [context](proc::time::range const &time_range, sync_source const &, channel_index_t const,
                      connector_index_t const co_idx, In const *const signal_ptr) mutable {
                static auto const input_co_idx = to_connector_index(input::value);

                if (co_idx == input_co_idx) {
                    context->set_time(proc::time{time_range}, co_idx);
                    context->copy_data_from(signal_ptr, time_range.length, co_idx);
                }
            });

        auto remove_processor = proc::make_remove_signal_processor<In>({to_connector_index(input::value)});

        auto send_processor = proc::make_send_signal_processor<Out>(
            [context, out_each = fast_each<Out *>{}](proc::time::range const &time_range, sync_source const &,
                                                     channel_index_t const, connector_index_t const co_idx,
                                                     Out *const signal_ptr) mutable {
                static auto const output_co_idx = to_connector_index(output::value);
                static auto const input_co_idx = to_connector_index(input::value);

                if (co_idx == output_co_idx) {
                    auto const *src_ptr = context->data(input_co_idx);
                    proc::time const &src_time = context->time(input_co_idx);
                    auto const src_offset = src_time ? time_range.frame - src_time.get<time::range>().frame : 0;
                    auto const &src_length = src_time ? src_time.get<time::range>().length : 0;

                    out_each.reset(signal_ptr, time_range.length);
                    while (yas_each_next(out_each)) {
                        auto const &idx = yas_each_index(out_each);
                        auto const src_idx = idx + src_offset;
                        auto const &src_value = (src_idx >= 0 && src_idx < src_length) ? src_ptr[src_idx] : 0;
                        yas_each_value(out_each) = static_cast<Out>(src_value);
                    }
                }
            });

        return std::vector<proc::processor_f>{{std::move(prepare_processor), std::move(receive_processor),
                                               std::move(remove_processor), std::move(send_processor)}};
    };

    return proc::module{std::move(make_processors)};
}

template <typename In, typename Out>
proc::module make_number_module() {
    auto make_processors = [] {
        auto context = std::make_shared<number_process_context<In, 2>>();

        auto prepare_processor = [context](time::range const &current_range, connector_map_t const &,
                                           connector_map_t const &,
                                           stream &) mutable { context->reset(current_range); };

        auto receive_processor = proc::make_receive_number_processor<In>(
            [context](proc::time::frame::type const &frame, channel_index_t const ch_idx,
                      connector_index_t const co_idx, In const &value) {
                if (co_idx == to_connector_index(input::value)) {
                    context->insert_input(frame, value, co_idx);
                }
            });

        auto remove_processor = proc::make_remove_number_processor<In>({to_connector_index(input::value)});

        auto send_processor = [context](time::range const &current_time_range, connector_map_t const &,
                                        connector_map_t const &output_connectors, stream &stream) {
            static auto const out_co_idx = to_connector_index(output::value);
            static auto const input_co_idx = to_connector_index(input::value);

            if (output_connectors.count(out_co_idx) > 0) {
                auto const &connector = output_connectors.at(out_co_idx);
                auto const &ch_idx = connector.channel_index;
                auto &channel = stream.has_channel(ch_idx) ? stream.channel(ch_idx) : stream.add_channel(ch_idx);
                for (auto const &input_pair : context->inputs()) {
                    channel.insert_event(make_frame_time(input_pair.first),
                                         number_event{static_cast<Out>(*input_pair.second.values[input_co_idx])});
                }
            }
        };

        return std::vector<proc::processor_f>{{std::move(prepare_processor), std::move(receive_processor),
                                               std::move(remove_processor), std::move(send_processor)}};
    };

    return proc::module{std::move(make_processors)};
}
}  // namespace yas::proc::cast
