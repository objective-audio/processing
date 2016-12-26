//
//  yas_processing_data_private.h
//

#pragma once

namespace yas {
struct processing::data::impl_base : base::impl {
    virtual std::type_info const &type() const = 0;
    virtual std::size_t sample_byte_count() const = 0;
    virtual std::size_t size() const = 0;
};

template <typename T>
struct processing::data::impl : impl_base {
    impl(std::vector<T> &&bytes) : _raw_data(std::move(bytes)), _raw_data_ref(_raw_data) {
    }

    impl(std::vector<T> &bytes) : _raw_data_ref(bytes) {
    }

    std::type_info const &type() const override {
        return typeid(T);
    }

    std::size_t sample_byte_count() const override {
        return sizeof(T);
    }

    std::size_t size() const override {
        return _raw_data_ref.size();
    }

    std::vector<T> &raw_data() {
        return _raw_data_ref;
    }

   private:
    std::vector<T> _raw_data;
    std::vector<T> &_raw_data_ref;
};

template <typename T>
processing::data::data(std::vector<T> &&bytes) : base(std::make_shared<impl<T>>(std::move(bytes))) {
}

template <typename T>
processing::data::data(std::vector<T> &bytes) : base(std::make_shared<impl<T>>(bytes)) {
}

template <typename T>
processing::data processing::make_data(std::size_t const length) {
    return processing::data{std::vector<T>(length)};
}

template <typename T>
std::vector<T> const &processing::get_raw(data const &data) {
    return data.impl_ptr<data::impl<T>>()->raw_data();
}

template <typename T>
std::vector<T> &processing::get_raw(data &data) {
    return data.impl_ptr<data::impl<T>>()->raw_data();
}
}
