//
//  yas_processing_receive_signal_processor.cpp
//

#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_channel.h"
#include "yas_processing_signal_event.h"
#include "yas_stl_utils.h"

using namespace yas;

template <typename T>
processing::processor_f processing::make_receive_signal_processor(processing::receive_signal_process_f<T> handler) {
    return [handler = std::move(handler)](time::range const &current_time_range,
                                          connector_map_t const &input_connectors, connector_map_t const &,
                                          stream &stream) {
        if (handler) {
            for (auto const &connector_pair : input_connectors) {
                auto const &connector_key = connector_pair.first;
                auto const &connector = connector_pair.second;

                auto const &ch_idx = connector.channel_index;

                if (stream.has_channel(ch_idx)) {
                    auto const &channel = stream.channel(ch_idx);

                    auto predicate = [](auto const &pair) {
                        if (pair.first.type() == typeid(time::range)) {
                            if (auto const signal = cast<processing::signal_event>(pair.second)) {
                                return signal.sample_type() == typeid(T);
                            }
                        }
                        return false;
                    };

                    auto const filtered_events = filter(channel.events(), predicate);

                    for (auto const &pair : filtered_events) {
                        time const &event_time = pair.first;
                        auto const &event_time_range = event_time.get<time::range>();
                        if (auto const time_range_opt = current_time_range.intersect(event_time_range)) {
                            auto const &time_range = *time_range_opt;
                            if (signal_event const signal = cast<processing::signal_event>(pair.second)) {
                                auto const *ptr = signal.data<T>();
                                auto const idx = time_range.frame - event_time_range.frame;
                                handler(time_range, ch_idx, connector_key, &ptr[idx]);
                            }
                        }
                    }
                }
            }
        }
    };
}

template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<double>);
template processing::processor_f processing::make_receive_signal_processor(processing::receive_signal_process_f<float>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<int64_t>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<int32_t>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<int16_t>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<int8_t>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<uint64_t>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<uint32_t>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<uint16_t>);
template processing::processor_f processing::make_receive_signal_processor(
    processing::receive_signal_process_f<uint8_t>);
