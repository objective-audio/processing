//
//  yas_processing_number_process_context_private.h
//

#pragma once

#include "yas_fast_each.h"

namespace yas {
namespace processing {
    template <typename T, std::size_t N>
    number_process_context<T, N>::number_process_context() : _last_values(N) {
    }

    template <typename T, std::size_t N>
    void number_process_context<T, N>::insert_input(frame_index_t const &frame, T const &value, std::size_t const idx) {
        if (this->_inputs.count(frame) == 0) {
            this->_inputs.emplace(frame, input{});
        }
        auto &input = this->_inputs.at(frame);
        input.values[idx] = value;
    }

    template <typename T, std::size_t N>
    void number_process_context<T, N>::update_last_values(input const &input) {
        auto each = make_fast_each(this->_last_values.data(), N);
        while (yas_fast_each_next(each)) {
            if (auto const &value = input.values[yas_fast_each_index(each)]) {
                yas_fast_each_value(each) = *value;
            }
        }
    }

    template <typename T, std::size_t N>
    void number_process_context<T, N>::reset() {
        this->_inputs.clear();
    }

    template <typename T, std::size_t N>
    std::map<frame_index_t, typename number_process_context<T, N>::input> const &
    number_process_context<T, N>::inputs() {
        return this->_inputs;
    }

    template <typename T, std::size_t N>
    T const *number_process_context<T, N>::last_values() const {
        return this->_last_values.data();
    }
}
}
