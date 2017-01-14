//
//  yas_processing_send_signal_processor.cpp
//

#include "yas_processing_send_signal_processor.h"
#include "yas_processing_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_channel.h"
#include "yas_processing_signal_event.h"
#include "yas_stl_utils.h"

using namespace yas;

template <typename T>
processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<T> handler) {
    return [handler = std::move(handler)](time::range const &current_time_range, connector_map_t const &,
                                          connector_map_t const &output_connectors, stream &stream) {
        if (handler) {
            for (auto const &connector_pair : output_connectors) {
                auto const &connector_key = connector_pair.first;
                auto const &connector = connector_pair.second;

                auto const &ch_idx = connector.channel_index;

                if (stream.has_channel(ch_idx)) {
                    auto &channel = stream.channel(ch_idx);
                    processing::time::range combined_time_range = current_time_range;

                    auto predicate = [&current_time_range](auto const &pair) {
                        if (pair.first.can_combine(current_time_range)) {
                            return true;
                        }
                        return false;
                    };

                    auto const filtered_events = channel.filtered_events<T, signal_event>(predicate);

                    if (filtered_events.size() > 0) {
                        for (auto const &pair : filtered_events) {
                            combined_time_range = *combined_time_range.combine(pair.first);
                        }

                        std::vector<T> vec(combined_time_range.length);
                        for (auto const &pair : filtered_events) {
                            auto const &time_range = pair.first;
                            auto const length = time_range.length;
                            auto const dst_idx = time_range.frame - combined_time_range.frame;
                            auto *dst_ptr = &vec[dst_idx];
                            signal_event const signal = pair.second;
                            signal.copy_to<T>(dst_ptr, length);
                        }

                        channel.erase_event<T, signal_event>(std::move(predicate));

                        handler(current_time_range, ch_idx, connector_key,
                                &vec[current_time_range.frame - combined_time_range.frame]);

                        channel.insert_event(time{combined_time_range}, signal_event{std::move(vec)});

                        return;
                    }
                } else {
                    stream.add_channel(ch_idx);
                }

                std::vector<T> vec(current_time_range.length);

                handler(current_time_range, ch_idx, connector_key, vec.data());

                auto &channel = stream.channel(ch_idx);
                channel.insert_event(time{current_time_range}, signal_event{std::move(vec)});
            }
        }
    };
}

template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<double>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<float>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<int64_t>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<int32_t>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<int16_t>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<int8_t>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<uint64_t>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<uint32_t>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<uint16_t>);
template processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<uint8_t>);
