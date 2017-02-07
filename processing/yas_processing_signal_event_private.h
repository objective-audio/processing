//
//  yas_processing_signal_event_private.h
//

#pragma once

#include "yas_processing_time.h"

namespace yas {
struct processing::signal_event::impl : event::impl {
    virtual std::type_info const &type() const = 0;
    virtual std::size_t sample_byte_count() const = 0;
    virtual std::size_t size() const = 0;
    virtual void resize(std::size_t const) = 0;
    virtual void reserve(std::size_t const) = 0;

    bool validate_time(time const &time) override {
        if (time.is_range_type()) {
            return time.get<time::range>().length == this->size();
        }

        return false;
    }
    
    virtual signal_event copy() = 0;
};

template <typename T>
struct processing::signal_event::type_impl : impl {
    type_impl(std::vector<T> &&bytes) : _vector(std::move(bytes)), _vector_ref(_vector) {
    }

    type_impl(std::vector<T> &bytes) : _vector_ref(bytes) {
    }

    signal_event copy() override {
        return signal_event{std::vector<T>{_vector_ref}};
    }

    std::type_info const &type() const override {
        return typeid(T);
    }

    std::size_t sample_byte_count() const override {
        return sizeof(T);
    }

    std::size_t size() const override {
        return this->_vector_ref.size();
    }

    void resize(std::size_t const size) override {
        this->_vector_ref.resize(size);
    }

    void reserve(std::size_t const size) override {
        this->_vector_ref.reserve(size);
    }

    std::vector<T> &vector() {
        return this->_vector_ref;
    }

   private:
    std::vector<T> _vector;
    std::vector<T> &_vector_ref;
};

template <typename T>
processing::signal_event::signal_event(std::vector<T> &&bytes)
    : event(std::make_shared<type_impl<T>>(std::move(bytes))) {
}

template <typename T>
processing::signal_event::signal_event(std::vector<T> &bytes) : event(std::make_shared<type_impl<T>>(bytes)) {
}

template <typename T>
std::vector<T> const &processing::signal_event::vector() const {
    return this->impl_ptr<signal_event::type_impl<T>>()->vector();
}

template <typename T>
std::vector<T> &processing::signal_event::vector() {
    return this->impl_ptr<signal_event::type_impl<T>>()->vector();
}

template <typename T>
T const *processing::signal_event::data() const {
    return this->impl_ptr<signal_event::type_impl<T>>()->vector().data();
}

template <typename T>
T *processing::signal_event::data() {
    return this->impl_ptr<signal_event::type_impl<T>>()->vector().data();
}

template <typename T>
void processing::signal_event::copy_from(T const *ptr, std::size_t const size) {
    this->resize(size);
    memcpy(this->data<T>(), ptr, size * sizeof(T));
}

template <typename T>
void processing::signal_event::copy_to(T *ptr, std::size_t const size) const {
    if (size > this->size()) {
        throw "out of range.";
    }

    memcpy(ptr, data<T>(), size * sizeof(T));
}

template <typename T>
processing::signal_event processing::make_signal_event(std::size_t const size) {
    return processing::signal_event{std::vector<T>(size)};
}

template <typename T>
processing::signal_event processing::make_signal_event(std::size_t const size, std::size_t const reserve) {
    auto vec = std::vector<T>(size);
    vec.reserve(reserve);
    return processing::signal_event{std::move(vec)};
}
}
