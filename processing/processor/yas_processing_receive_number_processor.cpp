//
//  yas_processing_receive_number_processor.cpp
//

#include "yas_processing_receive_number_processor.h"
#include <cpp_utils/yas_boolean.h>
#include "yas_processing_channel.h"
#include "yas_processing_number_event.h"
#include "yas_processing_stream.h"

using namespace yas;

template <typename T>
proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<T> handler) {
    return
        [handler = std::move(handler)](time::range const &current_time_range, connector_map_t const &input_connectors,
                                       connector_map_t const &, stream &stream) {
            if (handler) {
                for (auto const &connector_pair : input_connectors) {
                    auto const &connector_key = connector_pair.first;
                    auto const &connector = connector_pair.second;

                    auto const &ch_idx = connector.channel_index;

                    if (stream.has_channel(ch_idx)) {
                        auto const &channel = stream.channel(ch_idx);
                        auto const filtered_events = channel.filtered_events<T, proc::number_event>();

                        for (auto const &pair : filtered_events) {
                            auto const &event_frame = pair.first;
                            if (current_time_range.is_contain(event_frame)) {
                                number_event const &number_event = pair.second;
                                auto const &value = number_event.get<T>();
                                handler(event_frame, ch_idx, connector_key, value);
                            }
                        }
                    }
                }
            }
        };
}

template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<double>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<float>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<int64_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<int32_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<int16_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<int8_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<uint64_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<uint32_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<uint16_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<uint8_t>);
template proc::processor_f proc::make_receive_number_processor(proc::receive_number_process_f<boolean>);
