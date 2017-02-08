//
//  yas_processing_signal_event.h
//

#pragma once

#include "yas_processing_event.h"
#include "yas_processing_time.h"
#include <vector>

namespace yas {
namespace processing {
    class signal_event : public event {
       public:
        class impl;

        template <typename T>
        class type_impl;

        using time_type = time::range;

        template <typename T>
        explicit signal_event(std::vector<T> &&bytes);
        template <typename T>
        explicit signal_event(std::vector<T> &bytes);

        signal_event(std::nullptr_t);

        signal_event copy() const;

        std::type_info const &sample_type() const;
        std::size_t sample_byte_count() const;
        std::size_t size() const;
        void resize(std::size_t const);
        void reserve(std::size_t const);

        template <typename T>
        std::vector<T> const &vector() const;
        template <typename T>
        std::vector<T> &vector();

        template <typename T>
        T const *data() const;
        template <typename T>
        T *data();

        template <typename T>
        void copy_from(T const *, std::size_t const);
        template <typename T>
        void copy_to(T *, std::size_t const) const;
    };

    template <typename T>
    processing::signal_event make_signal_event(std::size_t const size);
    template <typename T>
    processing::signal_event make_signal_event(std::size_t const size, std::size_t const reserve);
}
}

#include "yas_processing_signal_event_private.h"
