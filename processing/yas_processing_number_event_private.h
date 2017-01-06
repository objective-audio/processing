//
//  yas_processing_number_event_private.h
//

#pragma once

#include "yas_processing_time.h"

namespace yas {
struct processing::number_event::impl : event::impl {
    virtual std::type_info const &type() const = 0;
    virtual std::size_t sample_byte_count() const = 0;

    bool validate_time(time const &time) override {
        return time.is_frame_type();
    }
};

template <typename T>
struct processing::number_event::type_impl : impl {
    type_impl(T &&value) : _value(std::move(value)) {
    }

    std::type_info const &type() const override {
        return typeid(T);
    }

    std::size_t sample_byte_count() const override {
        return sizeof(T);
    }

    T _value;
};

template <typename T>
processing::number_event::number_event(T value) : event(std::make_shared<type_impl<T>>(std::move(value))) {
}

template <typename T>
T const &processing::number_event::get() const {
    return impl_ptr<type_impl<T>>()->_value;
}
}
