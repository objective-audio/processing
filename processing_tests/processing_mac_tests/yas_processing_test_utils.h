//
//  yas_processing_test_utils.h
//

#pragma once

#include "yas_processing.h"
#include <functional>

using namespace yas;
using namespace yas::processing;

namespace yas {
namespace test {
    using process_f = std::function<void(processing::time::range const &, processing::connector_map_t const &,
                                         processing::connector_map_t const &, processing::stream &)>;

    template <typename T, typename Kind>
    static module make_signal_module(Kind const kind, channel_index_t const ch_idx) {
        auto module = processing::make_signal_module<T>(kind);
        module.connect_output(to_connector_index(math1::output::result), ch_idx);
        module.connect_input(to_connector_index(math1::input::parameter), ch_idx);

        return module;
    }

    template <typename T, typename Kind>
    static module make_number_module(Kind const kind, channel_index_t const ch_idx) {
        auto module = processing::make_number_module<T>(kind);
        module.connect_output(to_connector_index(math1::output::result), ch_idx);
        module.connect_input(to_connector_index(math1::input::parameter), ch_idx);

        return module;
    }

    template <typename T>
    static stream make_signal_stream(time::range const time_range, T const *const data,
                                     time::range const data_time_range, channel_index_t const ch_idx) {
        stream stream{sync_source{1, time_range.length}};

        auto &channel = stream.add_channel(ch_idx);

        signal_event phase_signal = make_signal_event<T>(data_time_range.length);
        auto *phase_data = phase_signal.data<T>();
        auto each = make_fast_each(phase_data, data_time_range.length);
        while (yas_fast_each_next(each)) {
            yas_fast_each_value(each) = data[yas_fast_each_index(each)];
        }

        channel.insert_event(processing::time{data_time_range}, std::move(phase_signal));

        return stream;
    }

    template <typename T>
    static stream make_number_stream(T const *const data, time::range const data_time_range,
                                     channel_index_t const ch_idx) {
        stream stream{sync_source{1, data_time_range.length}};

        auto &channel = stream.add_channel(ch_idx);

        auto each = make_fast_each(data_time_range.length);
        while (yas_fast_each_next(each)) {
            auto const &idx = yas_fast_each_index(each);
            channel.insert_event(make_frame_time(data_time_range.frame + idx), number_event(data[idx]));
        }

        return stream;
    }

    template <typename T>
    static stream make_signal_stream(time::range const time_range, T const *const left_data,
                                     time::range const left_time_range, channel_index_t const left_ch_idx,
                                     T const *const right_data, time::range const right_time_range,
                                     channel_index_t const right_ch_idx) {
        stream stream{sync_source{1, time_range.length}};

        {
            auto &channel = stream.add_channel(left_ch_idx);

            signal_event signal = make_signal_event<T>(left_time_range.length);
            auto *out_data = signal.data<T>();
            auto each = make_fast_each(out_data, left_time_range.length);
            while (yas_fast_each_next(each)) {
                yas_fast_each_value(each) = left_data[yas_fast_each_index(each)];
            }

            channel.insert_event(processing::time{left_time_range}, std::move(signal));
        }

        {
            auto &channel = stream.add_channel(right_ch_idx);

            signal_event signal = make_signal_event<T>(right_time_range.length);
            auto *out_data = signal.data<T>();
            auto each = make_fast_each(out_data, right_time_range.length);
            while (yas_fast_each_next(each)) {
                yas_fast_each_value(each) = right_data[yas_fast_each_index(each)];
            }

            channel.insert_event(processing::time{right_time_range}, std::move(signal));
        }

        return stream;
    }
}
}
