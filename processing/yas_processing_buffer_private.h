//
//  yas_processing_buffer_private.h
//

#pragma once

#include "yas_processing_time.h"

namespace yas {
struct processing::buffer::impl : event::impl {
    virtual std::type_info const &type() const = 0;
    virtual std::size_t sample_byte_count() const = 0;
    virtual std::size_t size() const = 0;
    virtual void resize(std::size_t const) = 0;

    bool validate_time(time const &time) override {
        if (time.is_range_type()) {
            return time.get<time::range>().length == size();
        }

        return false;
    }
};

template <typename T>
struct processing::buffer::type_impl : impl {
    type_impl(std::vector<T> &&bytes) : _vector(std::move(bytes)), _vector_ref(_vector) {
    }

    type_impl(std::vector<T> &bytes) : _vector_ref(bytes) {
    }

    std::type_info const &type() const override {
        return typeid(T);
    }

    std::size_t sample_byte_count() const override {
        return sizeof(T);
    }

    std::size_t size() const override {
        return _vector_ref.size();
    }

    void resize(std::size_t const size) override {
        _vector_ref.resize(size);
    }

    std::vector<T> &vector() {
        return _vector_ref;
    }

   private:
    std::vector<T> _vector;
    std::vector<T> &_vector_ref;
};

template <typename T>
processing::buffer::buffer(std::vector<T> &&bytes) : event(std::make_shared<type_impl<T>>(std::move(bytes))) {
}

template <typename T>
processing::buffer::buffer(std::vector<T> &bytes) : event(std::make_shared<type_impl<T>>(bytes)) {
}

template <typename T>
std::vector<T> const &processing::buffer::vector() const {
    return impl_ptr<buffer::type_impl<T>>()->vector();
}

template <typename T>
std::vector<T> &processing::buffer::vector() {
    return impl_ptr<buffer::type_impl<T>>()->vector();
}

template <typename T>
T const *processing::buffer::data() const {
    return impl_ptr<buffer::type_impl<T>>()->vector().data();
}

template <typename T>
T *processing::buffer::data() {
    return impl_ptr<buffer::type_impl<T>>()->vector().data();
}

template <typename T>
processing::buffer processing::make_buffer(std::size_t const size) {
    return processing::buffer{std::vector<T>(size)};
}

template <typename T>
processing::buffer processing::make_buffer(std::size_t const size, std::size_t const reserve) {
    auto vec = std::vector<T>(size);
    vec.reserve(reserve);
    return processing::buffer{std::move(vec)};
}
}
