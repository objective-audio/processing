//
//  yas_processing_signal_process_context_private.h
//

#pragma once

#include "yas_fast_each.h"

namespace yas {
namespace processing {
    template <typename T, std::size_t N>
    signal_process_context<T, N>::signal_process_context() {
        static_assert(N > 0, "N must greater than 0");

        this->_signals.reserve(N);

        auto each = make_fast_each(N);
        while (yas_fast_each_next(each)) {
            this->_signals.emplace_back(signal_event{std::vector<T>(0)});
            this->_times.emplace_back(processing::time{nullptr});
        }
    }

    template <typename T, std::size_t N>
    void signal_process_context<T, N>::reset(std::size_t const reserve_size) {
        for (auto &signal : this->_signals) {
            signal.reserve(reserve_size);
            signal.resize(0);
        }

        for (auto &time : this->_times) {
            time = nullptr;
        }
    }

    template <typename T, std::size_t N>
    std::vector<signal_event> const &signal_process_context<T, N>::signals() const {
        return this->_signals;
    }

    template <typename T, std::size_t N>
    std::vector<time> const &signal_process_context<T, N>::times() const {
        return this->_times;
    }

    template <typename T, std::size_t N>
    void signal_process_context<T, N>::copy_data_from(T const *ptr, std::size_t const size, std::size_t const idx) {
        signal_event &signal = this->_signals.at(idx);
        signal.copy_from<T>(ptr, size);
    }

    template <typename T, std::size_t N>
    T const *signal_process_context<T, N>::data(std::size_t const idx) const {
        signal_event const &signal = this->_signals.at(idx);
        return signal.data<T>();
    }

    template <typename T, std::size_t N>
    void signal_process_context<T, N>::set_time(processing::time time, std::size_t const idx) {
        if (!time.is_range_type()) {
            throw "time is not range type.";
        }
        this->_times.at(idx) = time;
    }

    template <typename T, std::size_t N>
    time const &signal_process_context<T, N>::time(std::size_t const idx) const {
        return this->_times.at(idx);
    }
}
}
