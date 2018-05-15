//
//  yas_processing_constant_module.cpp
//

#include "yas_processing_constant_module.h"
#include "yas_boolean.h"
#include "yas_fast_each.h"
#include "yas_processing_module.h"
#include "yas_processing_send_number_processor.h"
#include "yas_processing_send_signal_processor.h"

using namespace yas;

template <typename T> proc::module proc::make_signal_module(T value) {
  auto processor = proc::make_send_signal_processor<T>(
      [value = std::move(value), each = fast_each<T *>{}](
          proc::time::range const &time_range, sync_source const &,
          channel_index_t const, connector_index_t const,
          T *const signal_ptr) mutable {
        each.reset(signal_ptr, time_range.length);

        while (yas_each_next(each)) {
          yas_each_value(each) = value;
        }
      });

  return proc::module{{std::move(processor)}};
}

template proc::module proc::make_signal_module(double);
template proc::module proc::make_signal_module(float);
template proc::module proc::make_signal_module(int64_t);
template proc::module proc::make_signal_module(int32_t);
template proc::module proc::make_signal_module(int16_t);
template proc::module proc::make_signal_module(int8_t);
template proc::module proc::make_signal_module(uint64_t);
template proc::module proc::make_signal_module(uint32_t);
template proc::module proc::make_signal_module(uint16_t);
template proc::module proc::make_signal_module(uint8_t);
template proc::module proc::make_signal_module(boolean);

template <typename T> proc::module proc::make_number_module(T value) {
  auto processor = proc::make_send_number_processor<T>(
      [value](proc::time::range const &time_range, sync_source const &,
              channel_index_t const, connector_index_t const) {
        return number_event::value_map_t<T>{{time_range.frame, value}};
      });

  return proc::module{{std::move(processor)}};
}

template proc::module proc::make_number_module(double);
template proc::module proc::make_number_module(float);
template proc::module proc::make_number_module(int64_t);
template proc::module proc::make_number_module(int32_t);
template proc::module proc::make_number_module(int16_t);
template proc::module proc::make_number_module(int8_t);
template proc::module proc::make_number_module(uint64_t);
template proc::module proc::make_number_module(uint32_t);
template proc::module proc::make_number_module(uint16_t);
template proc::module proc::make_number_module(uint8_t);
template proc::module proc::make_number_module(boolean);
