//
//  yas_processing_number_event_private.h
//

#pragma once

#include "yas_processing_time.h"

namespace yas {
struct proc::number_event::impl : event::impl {
  virtual std::type_info const &type() const = 0;
  virtual std::size_t sample_byte_count() const = 0;

  bool validate_time(time const &time) override { return time.is_frame_type(); }
};
} // namespace yas
