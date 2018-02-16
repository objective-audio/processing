//
//  yas_processing_number_event.cpp
//

#include "yas_processing_number_event.h"
#include "yas_boolean.h"

using namespace yas;

namespace yas::processing {
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

    virtual bool is_equal(std::shared_ptr<base::impl> const &rhs) const override {
        if (auto casted_rhs = std::dynamic_pointer_cast<type_impl<T>>(rhs)) {
            return this->_value == casted_rhs->_value;
        }

        return false;
    }

    event copy() override {
        return number_event{_value};
    }

    T _value;
};
}

template <typename T>
processing::number_event::number_event(T value) : event(std::make_shared<type_impl<T>>(std::move(value))) {
}

template processing::number_event::number_event(double);
template processing::number_event::number_event(float);
template processing::number_event::number_event(int64_t);
template processing::number_event::number_event(int32_t);
template processing::number_event::number_event(int16_t);
template processing::number_event::number_event(int8_t);
template processing::number_event::number_event(uint64_t);
template processing::number_event::number_event(uint32_t);
template processing::number_event::number_event(uint16_t);
template processing::number_event::number_event(uint8_t);
template processing::number_event::number_event(boolean);

processing::number_event::number_event(std::nullptr_t) : event(nullptr) {
}

std::type_info const &processing::number_event::sample_type() const {
    return this->impl_ptr<impl>()->type();
}

std::size_t processing::number_event::sample_byte_count() const {
    return this->impl_ptr<impl>()->sample_byte_count();
}

template <typename T>
T const &processing::number_event::get() const {
    return this->impl_ptr<type_impl<T>>()->_value;
}

template double const &processing::number_event::get() const;
template float const &processing::number_event::get() const;
template int64_t const &processing::number_event::get() const;
template int32_t const &processing::number_event::get() const;
template int16_t const &processing::number_event::get() const;
template int8_t const &processing::number_event::get() const;
template uint64_t const &processing::number_event::get() const;
template uint32_t const &processing::number_event::get() const;
template uint16_t const &processing::number_event::get() const;
template uint8_t const &processing::number_event::get() const;
template boolean const &processing::number_event::get() const;

template <typename T>
processing::number_event processing::make_number_event(T value) {
    return number_event{std::move(value)};
}

template processing::number_event processing::make_number_event(double);
template processing::number_event processing::make_number_event(float);
template processing::number_event processing::make_number_event(int64_t);
template processing::number_event processing::make_number_event(int32_t);
template processing::number_event processing::make_number_event(int16_t);
template processing::number_event processing::make_number_event(int8_t);
template processing::number_event processing::make_number_event(uint64_t);
template processing::number_event processing::make_number_event(uint32_t);
template processing::number_event processing::make_number_event(uint16_t);
template processing::number_event processing::make_number_event(uint8_t);
template processing::number_event processing::make_number_event(boolean);
