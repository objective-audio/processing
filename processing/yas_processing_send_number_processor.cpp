//
//  yas_processing_send_number_processor.cpp
//

#include "yas_processing_send_number_processor.h"
#include "yas_processing_stream.h"
#include "yas_processing_time.h"
#include "yas_boolean.h"

using namespace yas;

template <typename T>
processing::processor_f processing::make_send_number_processor(send_number_process_f<T> handler) {
    return [handler = std::move(handler)](time::range const &current_time_range, connector_map_t const &,
                                          connector_map_t const &output_connectors, stream &stream) {
        if (handler) {
            for (auto const &connector_pair : output_connectors) {
                auto const &con_idx = connector_pair.first;
                auto const &connector = connector_pair.second;

                auto const &ch_idx = connector.channel_index;
                auto &channel = stream.add_channel(ch_idx);

                if (channel.events().size() > 0) {
                    channel.erase_event<T, number_event>([&erase_range = current_time_range](auto const &pair) {
                        return erase_range.is_contain(pair.first);
                    });
                }

                auto map = handler(current_time_range, stream.sync_source(), ch_idx, con_idx);

                for (auto const &number_pair : map) {
                    channel.insert_event(make_frame_time(number_pair.first), make_number_event<T>(number_pair.second));
                }
            }
        }
    };
}

template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<double>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<float>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<int64_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<int32_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<int16_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<int8_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<uint64_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<uint32_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<uint16_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<uint8_t>);
template processing::processor_f processing::make_send_number_processor(processing::send_number_process_f<boolean>);
