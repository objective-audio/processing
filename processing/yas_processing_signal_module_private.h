//
//  yas_processing_signal_module_private.h
//

#pragma once

#include "yas_processing_stream.h"
#include "yas_processing_time_range.h"
#include "yas_processing_data.h"
#include "yas_processing_channel.h"
#include "yas_stl_utils.h"
#include <vector>

namespace yas {
struct processing::signal_module::impl_base : module::impl {
    impl_base(processing::time_range &&time_range) : module::impl(std::move(time_range)) {
    }
};

template <typename T>
struct processing::signal_module::impl : impl_base {
    impl(args<T> &&args)
        : impl_base(std::move(args.time_range)),
          _send_signal_handler(std::move(args.send_signal_handler)),
          _receive_signal_handler(std::move(args.receive_signal_handler)) {
    }

    virtual void process(stream &stream) override final {
        auto const current_time_range_opt = time_range().intersect(stream.time_range());

        if (!current_time_range_opt) {
            return;
        }

        auto const &current_time_range = *current_time_range_opt;

        if (_receive_signal_handler) {
            for (auto const &connector_pair : input_connectors()) {
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
                            _receive_signal_handler(time_range, ch_idx, connector_key, &ptr[idx]);
                        }
                    }
                }
            }
        }

        if (_send_signal_handler) {
            for (auto const &connector_pair : output_connectors()) {
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

                    auto const filtered_datas = filter(channel.datas(), predicate);

                    if (filtered_datas.size() > 0) {
                        std::vector<T> vec(combined_time_range.length);
                        for (auto const &pair : filtered_datas) {
                            auto const length = pair.first.length;
                            auto const dst_idx = pair.first.start_frame - combined_time_range.start_frame;
                            auto *dst_ptr = &vec[dst_idx];
                            auto const *src_ptr = get_raw<T>(pair.second).data();
                            memcpy(dst_ptr, src_ptr, length * sizeof(T));
                        }

                        channel.erase_data_if(std::move(predicate));

                        _send_signal_handler(current_time_range, ch_idx, connector_key,
                                             &vec[current_time_range.start_frame - combined_time_range.start_frame]);

                        channel.insert_data(combined_time_range, std::move(vec));

                        return;
                    }
                } else {
                    stream.insert_channel(ch_idx);
                }

                std::vector<T> vec(current_time_range.length);

                _send_signal_handler(current_time_range, ch_idx, connector_key, vec.data());

                auto &channel = stream.channel(ch_idx);
                channel.insert_data(current_time_range, std::move(vec));
            }
        }
    }

   private:
    send_signal_f<T> _send_signal_handler;
    receive_signal_f<T> _receive_signal_handler;
};
}
