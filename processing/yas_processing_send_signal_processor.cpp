//
//  yas_processing_send_signal_processor.cpp
//

#include "yas_processing_send_signal_processor.h"
#include "yas_processing_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_channel.h"
#include "yas_processing_buffer.h"
#include "yas_stl_utils.h"

using namespace yas;

template <typename T>
processing::processor_f processing::make_send_signal_processor(processing::send_signal_process_f<T> handler) {
    return [handler = std::move(handler)](time::range const &current_time_range,
                                          connector_map_t const &input_connectors,
                                          connector_map_t const &output_connectors, stream &stream) {
        if (handler) {
            for (auto const &connector_pair : output_connectors) {
                auto const &connector_key = connector_pair.first;
                auto const &connector = connector_pair.second;

                auto const &ch_idx = connector.channel_index;

                if (stream.has_channel(ch_idx)) {
                    auto &channel = stream.channel(ch_idx);
                    processing::time::range combined_time_range = current_time_range;

                    auto predicate = [&current_time_range, &combined_time_range](auto const &pair) {
                        time const &time = pair.first;
                        if (time.type() == typeid(time::range)) {
                            auto const &time_range = time.get<time::range>();
                            if (pair.second.sample_type() == typeid(T) && time_range.can_combine(current_time_range)) {
                                combined_time_range = *combined_time_range.combine(time_range);
                                return true;
                            }
                        }
                        return false;
                    };

                    auto const filtered_buffers = filter(channel.buffers(), predicate);

                    if (filtered_buffers.size() > 0) {
                        std::vector<T> vec(combined_time_range.length);
                        for (auto const &pair : filtered_buffers) {
                            time const &time = pair.first;
                            auto const &time_range = time.get<time::range>();
                            auto const length = time_range.length;
                            auto const dst_idx = time_range.frame - combined_time_range.frame;
                            auto *dst_ptr = &vec[dst_idx];
                            processing::buffer const &buffer = pair.second;
                            auto const *src_ptr = buffer.data<T>();
                            memcpy(dst_ptr, src_ptr, length * sizeof(T));
                        }

                        channel.erase_buffer_if(std::move(predicate));

                        handler(current_time_range, ch_idx, connector_key,
                                 &vec[current_time_range.frame - combined_time_range.frame]);

                        channel.insert_buffer(time{combined_time_range}, std::move(vec));

                        return;
                    }
                } else {
                    stream.add_channel(ch_idx);
                }

                std::vector<T> vec(current_time_range.length);

                handler(current_time_range, ch_idx, connector_key, vec.data());

                auto &channel = stream.channel(ch_idx);
                channel.insert_buffer(time{current_time_range}, std::move(vec));
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
