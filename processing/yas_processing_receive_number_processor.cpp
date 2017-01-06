//
//  yas_processing_receive_number_processor.cpp
//

#include "yas_processing_receive_number_processor.h"
#include "yas_processing_number_event.h"
#include "yas_processing_channel.h"
#include "yas_processing_stream.h"
#include "yas_stl_utils.h"

using namespace yas;

template <typename T>
processing::processor_f processing::make_receive_number_processor(processing::receive_number_process_f<T> handler) {
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

                    auto predicate = [&current_time_range](auto const &pair) {
                        time const &time = pair.first;
                        if (time.type() == typeid(time::frame)) {
                            auto const &frame = time.get<time::frame>();
                            if (current_time_range.is_contain(frame)) {
                                if (auto const number = cast<processing::number_event>(pair.second)) {
                                    return number.sample_type() == typeid(T);
                                }
                            }
                        }
                        return false;
                    };

                    auto const filtered_events = filter(channel.events(), predicate);

                    for (auto const &pair : filtered_events) {
                        time const &event_time = pair.first;
                        auto const &event_frame = event_time.get<time::frame>();
                        if (number_event const number_event = cast<processing::number_event>(pair.second)) {
                            auto const &value = number_event.get<T>();
                            handler(event_frame, ch_idx, connector_key, value);
                        }
                    }
                }
            }
        }
    };
}

template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<double>);
template processing::processor_f processing::make_receive_number_processor(processing::receive_number_process_f<float>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<int64_t>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<int32_t>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<int16_t>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<int8_t>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<uint64_t>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<uint32_t>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<uint16_t>);
template processing::processor_f processing::make_receive_number_processor(
    processing::receive_number_process_f<uint8_t>);
