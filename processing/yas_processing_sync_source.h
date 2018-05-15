//
//  yas_processing_sync_source.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas::proc {
struct sync_source {
  sample_rate_t const sample_rate;
  length_t const slice_length;

  sync_source(sample_rate_t const, length_t const);

  bool operator==(sync_source const &) const;
  bool operator!=(sync_source const &) const;
};
} // namespace yas::proc
