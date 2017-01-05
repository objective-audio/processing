//
//  yas_processing_receive_signal_processor.cpp
//

#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_channel.h"
#include "yas_processing_buffer.h"
#include "yas_stl_utils.h"

using namespace yas;

namespace yas {
namespace processing {
    template <typename T>
    struct receive_signal_processor_impl : processor::impl {
        receive_signal_processor_impl(processing::receive_signal_process_f<T> &&handler)
            : _handler(std::move(handler)) {
        }

        void process(time::range const &current_time_range, connector_map_t const &input_connectors,
                     connector_map_t const &, stream &stream) override {
            if (_handler) {
                for (auto const &connector_pair : input_connectors) {
                    auto const &connector_key = connector_pair.first;
                    auto const &connector = connector_pair.second;

                    auto const &ch_idx = connector.channel_index;

                    if (stream.has_channel(ch_idx)) {
                        auto const &channel = stream.channel(ch_idx);

                        auto predicate = [](auto const &pair) { return pair.second.sample_type() == typeid(T); };

                        auto const filtered_buffers = filter(channel.buffers(), predicate);

                        for (auto const &pair : filtered_buffers) {
                            time const &buf_time = pair.first;
                            auto const &buf_time_range = buf_time.get<time::range>();
                            if (auto const time_range_opt = current_time_range.intersect(buf_time_range)) {
                                auto const &time_range = *time_range_opt;
                                processing::buffer const &buffer = pair.second;
                                auto const *ptr = buffer.data<T>();
                                auto const idx = time_range.frame - buf_time_range.frame;
                                _handler(time_range, ch_idx, connector_key, &ptr[idx]);
                            }
                        }
                    }
                }
            }
        }

       private:
        processing::receive_signal_process_f<T> _handler;
    };
}
}

template <typename T>
processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<T> handler) {
    return processing::processor{std::make_shared<receive_signal_processor_impl<T>>(std::move(handler))};
}

template processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<double>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<float>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<int64_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<int32_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<int16_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<int8_t>);
template processing::processor processing::make_receive_signal_processor(
    processing::receive_signal_process_f<uint64_t>);
template processing::processor processing::make_receive_signal_processor(
    processing::receive_signal_process_f<uint32_t>);
template processing::processor processing::make_receive_signal_processor(
    processing::receive_signal_process_f<uint16_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_process_f<uint8_t>);
