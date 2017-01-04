//
//  yas_processing_buffer_private.h
//

#pragma once

namespace yas {
struct processing::buffer::impl_base : base::impl {
    virtual std::type_info const &type() const = 0;
    virtual std::size_t sample_byte_count() const = 0;
    virtual std::size_t size() const = 0;
};

template <typename T>
struct processing::buffer::impl : impl_base {
    impl(std::vector<T> &&bytes) : _vector(std::move(bytes)), _vector_ref(_vector) {
    }

    impl(std::vector<T> &bytes) : _vector_ref(bytes) {
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

    std::vector<T> &vector() {
        return _vector_ref;
    }

   private:
    std::vector<T> _vector;
    std::vector<T> &_vector_ref;
};

template <typename T>
processing::buffer::buffer(std::vector<T> &&bytes) : base(std::make_shared<impl<T>>(std::move(bytes))) {
}

template <typename T>
processing::buffer::buffer(std::vector<T> &bytes) : base(std::make_shared<impl<T>>(bytes)) {
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

template <typename T>
std::vector<T> const &processing::get_vector(buffer const &buffer) {
    return buffer.impl_ptr<buffer::impl<T>>()->vector();
}

template <typename T>
std::vector<T> &processing::get_vector(buffer &buffer) {
    return buffer.impl_ptr<buffer::impl<T>>()->vector();
}
}
