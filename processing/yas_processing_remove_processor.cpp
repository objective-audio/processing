//
//  yas_processing_remove_processor.cpp
//

#include "yas_processing_remove_processor.h"
#include "yas_processing_number_event.h"
#include "yas_processing_channel.h"
#include "yas_processing_stream.h"
#include "yas_stl_utils.h"

using namespace yas;

template <typename T>
processing::processor_f processing::make_remove_number_processor() {
    return [](time::range const &time_range, connector_map_t const &input_connectors, connector_map_t const &,
              stream &stream) {
        for (auto const &connector_pair : input_connectors) {
            auto const &connector = connector_pair.second;
            auto const &ch_idx = connector.channel_index;

            if (stream.has_channel(ch_idx)) {
                auto &channel = stream.channel(ch_idx);

                auto predicate = [&time_range](auto const &pair) {
                    time const &time = pair.first;
                    if (time.type() == typeid(time::frame)) {
                        auto const &frame = time.get<time::frame>();
                        if (time_range.is_contain(frame)) {
                            if (auto const number = cast<processing::number_event>(pair.second)) {
                                return number.sample_type() == typeid(T);
                            }
                        }
                    }
                    return false;
                };

                channel.erase_event_if(predicate);
            }
        }
    };
}

template processing::processor_f processing::make_remove_number_processor<double>();
template processing::processor_f processing::make_remove_number_processor<float>();
template processing::processor_f processing::make_remove_number_processor<int64_t>();
template processing::processor_f processing::make_remove_number_processor<int32_t>();
template processing::processor_f processing::make_remove_number_processor<int16_t>();
template processing::processor_f processing::make_remove_number_processor<int8_t>();
template processing::processor_f processing::make_remove_number_processor<uint64_t>();
template processing::processor_f processing::make_remove_number_processor<uint32_t>();
template processing::processor_f processing::make_remove_number_processor<uint16_t>();
template processing::processor_f processing::make_remove_number_processor<uint8_t>();
