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

namespace yas {
namespace processing {
    template <typename T>
    struct send_signal_processor_impl : processor::impl {
        send_signal_processor_impl(processing::send_signal_f<T> &&handler) : _handler(std::move(handler)) {
        }

        void process(time_range const &current_time_range, module const &module, stream &stream) override {
            if (_handler) {
                for (auto const &connector_pair : module.output_connectors()) {
                    auto const &connector_key = connector_pair.first;
                    auto const &connector = connector_pair.second;

                    auto const &ch_idx = connector.channel_index;

                    if (stream.has_channel(ch_idx)) {
                        auto &channel = stream.channel(ch_idx);
                        processing::time_range combined_time_range = current_time_range;

                        auto predicate = [&current_time_range, &combined_time_range](auto const &pair) {
                            if (pair.second.sample_type() == typeid(T) && pair.first.can_combine(current_time_range)) {
                                combined_time_range = *combined_time_range.combine(pair.first);
                                return true;
                            }
                            return false;
                        };

                        auto const filtered_buffers = filter(channel.buffers(), predicate);

                        if (filtered_buffers.size() > 0) {
                            std::vector<T> vec(combined_time_range.length);
                            for (auto const &pair : filtered_buffers) {
                                auto const length = pair.first.length;
                                auto const dst_idx = pair.first.start_frame - combined_time_range.start_frame;
                                auto *dst_ptr = &vec[dst_idx];
                                auto const *src_ptr = get_vector<T>(pair.second).data();
                                memcpy(dst_ptr, src_ptr, length * sizeof(T));
                            }

                            channel.erase_buffer_if(std::move(predicate));

                            _handler(current_time_range, ch_idx, connector_key,
                                     &vec[current_time_range.start_frame - combined_time_range.start_frame]);

                            channel.insert_buffer(combined_time_range, std::move(vec));

                            return;
                        }
                    } else {
                        stream.insert_channel(ch_idx);
                    }

                    std::vector<T> vec(current_time_range.length);

                    _handler(current_time_range, ch_idx, connector_key, vec.data());

                    auto &channel = stream.channel(ch_idx);
                    channel.insert_buffer(current_time_range, std::move(vec));
                }
            }
        }

       private:
        processing::send_signal_f<T> _handler;
    };
}
}

template <typename T>
processing::processor processing::make_send_signal_processor(processing::send_signal_f<T> handler) {
    return processing::processor{std::make_shared<send_signal_processor_impl<T>>(std::move(handler))};
}

template processing::processor processing::make_send_signal_processor(processing::send_signal_f<double>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<float>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<int64_t>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<int32_t>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<int16_t>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<int8_t>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<uint64_t>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<uint32_t>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<uint16_t>);
template processing::processor processing::make_send_signal_processor(processing::send_signal_f<uint8_t>);
