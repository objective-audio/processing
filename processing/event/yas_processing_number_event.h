//
//  yas_processing_number_event.h
//

#pragma once

#include <map>
#include "yas_processing_event.h"
#include "yas_processing_time.h"

namespace yas::proc {
struct number_event : event {
    using time_type = time::frame;

    template <typename T>
    using value_map_t = std::multimap<time::frame::type, T>;

    std::type_info const &sample_type() const;
    std::size_t sample_byte_count() const;

    template <typename T>
    T const &get() const;

    std::shared_ptr<event> copy() const override;
    bool validate_time(time const &) const override;
    bool is_equal(std::shared_ptr<event> const &) const override;

   private:
    class impl;

    template <typename T>
    class type_impl;

    std::shared_ptr<impl> _impl;

    template <typename T>
    explicit number_event(T);

    number_event(number_event const &) = delete;
    number_event(number_event &&) = delete;
    number_event &operator=(number_event const &) = delete;
    number_event &operator=(number_event &&) = delete;

   public:
    template <typename T>
    static proc::number_event_ptr make_shared(T);
};
}  // namespace yas::proc

#include "yas_processing_number_event_private.h"
