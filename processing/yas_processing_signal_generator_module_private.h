//
//  yas_processing_signal_generator_module_private.h
//

#pragma once

#include "yas_processing_stream.h"
#include "yas_processing_time_range.h"
#include "yas_processing_data.h"
#include "yas_processing_channel.h"
#include "yas_stl_utils.h"
#include <vector>

namespace yas {
struct processing::signal_generator_module::impl_base : module::impl {
    impl_base(processing::time_range &&time_range)
        : module::impl(processing::module::impl::args{.time_range = std::move(time_range),
                                                      .output_connectors = {{result_key, {}}}}) {
    }
};

template <typename T>
struct processing::signal_generator_module::impl : impl_base {
    impl(processing::time_range &&time_range, process_f<T> &&handler)
        : impl_base(std::move(time_range)),
          _result_ch_idx(output_connectors().at(result_key).channel),
          _process_handler(std::move(handler)) {
    }

    void process_signal(processing::time_range const &time_range, T *signal_ptr) {
        if (_process_handler) {
            _process_handler(time_range, signal_ptr);
        }
    }

    virtual void process(stream &stream) override final {
        if (!_result_ch_idx) {
            return;
        }

        auto const current_time_range_opt = time_range().intersect(stream.time_range());

        if (!current_time_range_opt) {
            return;
        }

        auto const &current_time_range = *current_time_range_opt;
        auto const &result_ch_idx = *_result_ch_idx;

        if (stream.has_channel(result_ch_idx)) {
            auto &channel = stream.channel(result_ch_idx);
            processing::time_range combined_time_range = current_time_range;

            auto predicate = [&current_time_range, &combined_time_range](auto const &pair) {
                if (pair.second.sample_type() == typeid(T) && pair.first.can_combine(current_time_range)) {
                    combined_time_range = *combined_time_range.combine(pair.first);
                    return true;
                }
                return false;
            };

            auto const &filtered_datas = filter(channel.datas(), predicate);

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

                process_signal(current_time_range,
                               &vec[current_time_range.start_frame - combined_time_range.start_frame]);

                channel.insert_data(combined_time_range, std::move(vec));

                return;
            }
        } else {
            stream.insert_channel(result_ch_idx);
        }

        std::vector<T> vec(current_time_range.length);

        process_signal(current_time_range, vec.data());

        auto &channel = stream.channel(result_ch_idx);
        channel.insert_data(current_time_range, std::move(vec));
    }

   private:
    connector::channel_opt_t const &_result_ch_idx;
    process_f<T> _process_handler;
};
}
