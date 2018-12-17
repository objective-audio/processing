//
//  yas_processing_number_event.h
//

#pragma once

#include "yas_processing_event.h"
#include "yas_processing_time.h"

namespace yas::proc {
class number_event : public event {
   public:
    class impl;

    template <typename T>
    class type_impl;

    using time_type = time::frame;

    template <typename T>
    using value_map_t = std::multimap<time::frame::type, T>;

    template <typename T>
    explicit number_event(T);

    number_event(std::nullptr_t);

    std::type_info const &sample_type() const;
    std::size_t sample_byte_count() const;

    template <typename T>
    T const &get() const;
};

template <typename T>
number_event make_number_event(T);
}  // namespace yas::proc

#include "yas_processing_number_event_private.h"
