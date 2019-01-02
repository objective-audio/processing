//
//  yas_processing_receive_signal_processor.cpp
//

#include "yas_processing_receive_signal_processor.h"
#include <cpp_utils/yas_boolean.h>
#include "yas_processing_channel.h"
#include "yas_processing_module.h"
#include "yas_processing_processor.h"
#include "yas_processing_signal_event.h"

using namespace yas;

template <typename T>
proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<T> handler) {
    return
        [handler = std::move(handler)](time::range const &current_time_range, connector_map_t const &input_connectors,
                                       connector_map_t const &, stream &stream) {
            if (handler) {
                for (auto const &connector_pair : input_connectors) {
                    auto const &co_idx = connector_pair.first;
                    auto const &connector = connector_pair.second;

                    auto const &ch_idx = connector.channel_index;

                    if (stream.has_channel(ch_idx)) {
                        auto const &channel = stream.channel(ch_idx);
                        auto const filtered_events = channel.filtered_events<T, proc::signal_event>();

                        for (auto const &pair : filtered_events) {
                            auto const &event_time_range = pair.first;
                            if (auto const time_range_opt = current_time_range.intersected(event_time_range)) {
                                auto const &time_range = *time_range_opt;
                                signal_event const &signal = pair.second;
                                auto const *ptr = signal.data<T>();
                                auto const idx = time_range.frame - event_time_range.frame;
                                handler(time_range, stream.sync_source(), ch_idx, co_idx, &ptr[idx]);
                            }
                        }
                    }
                }
            }
        };
}

template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<double>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<float>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<int64_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<int32_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<int16_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<int8_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<uint64_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<uint32_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<uint16_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<uint8_t>);
template proc::processor_f proc::make_receive_signal_processor(proc::receive_signal_process_f<boolean>);
