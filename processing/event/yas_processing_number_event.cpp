//
//  yas_processing_number_event.cpp
//

#include "yas_processing_number_event.h"
#include "yas_boolean.h"

using namespace yas;

namespace yas::proc {
template <typename T> struct proc::number_event::type_impl : impl {
  type_impl(T &&value) : _value(std::move(value)) {}

  std::type_info const &type() const override { return typeid(T); }

  std::size_t sample_byte_count() const override { return sizeof(T); }

  virtual bool is_equal(std::shared_ptr<base::impl> const &rhs) const override {
    if (auto casted_rhs = std::dynamic_pointer_cast<type_impl<T>>(rhs)) {
      return this->_value == casted_rhs->_value;
    }

    return false;
  }

  event copy() override { return number_event{_value}; }

  T _value;
};
} // namespace yas::proc

template <typename T>
proc::number_event::number_event(T value)
    : event(std::make_shared<type_impl<T>>(std::move(value))) {}

template proc::number_event::number_event(double);
template proc::number_event::number_event(float);
template proc::number_event::number_event(int64_t);
template proc::number_event::number_event(int32_t);
template proc::number_event::number_event(int16_t);
template proc::number_event::number_event(int8_t);
template proc::number_event::number_event(uint64_t);
template proc::number_event::number_event(uint32_t);
template proc::number_event::number_event(uint16_t);
template proc::number_event::number_event(uint8_t);
template proc::number_event::number_event(boolean);

proc::number_event::number_event(std::nullptr_t) : event(nullptr) {}

std::type_info const &proc::number_event::sample_type() const {
  return this->impl_ptr<impl>()->type();
}

std::size_t proc::number_event::sample_byte_count() const {
  return this->impl_ptr<impl>()->sample_byte_count();
}

template <typename T> T const &proc::number_event::get() const {
  return this->impl_ptr<type_impl<T>>()->_value;
}

template double const &proc::number_event::get() const;
template float const &proc::number_event::get() const;
template int64_t const &proc::number_event::get() const;
template int32_t const &proc::number_event::get() const;
template int16_t const &proc::number_event::get() const;
template int8_t const &proc::number_event::get() const;
template uint64_t const &proc::number_event::get() const;
template uint32_t const &proc::number_event::get() const;
template uint16_t const &proc::number_event::get() const;
template uint8_t const &proc::number_event::get() const;
template boolean const &proc::number_event::get() const;

template <typename T> proc::number_event proc::make_number_event(T value) {
  return number_event{std::move(value)};
}

template proc::number_event proc::make_number_event(double);
template proc::number_event proc::make_number_event(float);
template proc::number_event proc::make_number_event(int64_t);
template proc::number_event proc::make_number_event(int32_t);
template proc::number_event proc::make_number_event(int16_t);
template proc::number_event proc::make_number_event(int8_t);
template proc::number_event proc::make_number_event(uint64_t);
template proc::number_event proc::make_number_event(uint32_t);
template proc::number_event proc::make_number_event(uint16_t);
template proc::number_event proc::make_number_event(uint8_t);
template proc::number_event proc::make_number_event(boolean);
