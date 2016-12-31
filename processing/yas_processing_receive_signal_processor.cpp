
    //
//  yas_processing_receive_signal_processor.cpp
//

#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_channel.h"
#include "yas_processing_data.h"
#include "yas_stl_utils.h"

using namespace yas;

namespace yas {
namespace processing {
    template <typename T>
    struct receive_signal_processor_impl : processor::impl {
        receive_signal_processor_impl(processing::receive_signal_f<T> &&handler) : _handler(std::move(handler)) {
        }

        void process(module const &module, time_range const &current_time_range, stream &stream) override {
            if (_handler) {
                auto const current_time_range_opt = module.time_range().intersect(stream.time_range());
                if (!current_time_range_opt) {
                    return;
                }
                auto const &current_time_range = *current_time_range_opt;

                for (auto const &connector_pair : module.input_connectors()) {
                    auto const &connector_key = connector_pair.first;
                    auto const &connector = connector_pair.second;

                    auto const &ch_idx = connector.channel_index;

                    if (stream.has_channel(ch_idx)) {
                        auto const &channel = stream.channel(ch_idx);

                        auto predicate = [](auto const &pair) { return pair.second.sample_type() == typeid(T); };

                        auto const filtered_datas = filter(channel.datas(), predicate);

                        for (auto const &pair : filtered_datas) {
                            if (auto const time_range_opt = current_time_range.intersect(pair.first)) {
                                auto const &time_range = *time_range_opt;
                                auto const *ptr = get_raw<T>(pair.second).data();
                                auto const idx = time_range.start_frame - current_time_range.start_frame;
                                _handler(time_range, ch_idx, connector_key, &ptr[idx]);
                            }
                        }
                    }
                }
            }
        }

       private:
        processing::receive_signal_f<T> _handler;
    };
}
}

template <typename T>
processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<T> handler) {
    return processing::processor{std::make_shared<receive_signal_processor_impl<T>>(std::move(handler))};
}

template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<double>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<float>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<int64_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<int32_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<int16_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<int8_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<uint64_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<uint32_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<uint16_t>);
template processing::processor processing::make_receive_signal_processor(processing::receive_signal_f<uint8_t>);
;
