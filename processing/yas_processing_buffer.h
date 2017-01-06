//
//  yas_processing_buffer.h
//

#pragma once

#include "yas_processing_event.h"
#include <vector>

namespace yas {
namespace processing {
    class buffer : public event {
       public:
        class impl;

        template <typename T>
        class type_impl;

        template <typename T>
        explicit buffer(std::vector<T> &&bytes);
        template <typename T>
        explicit buffer(std::vector<T> &bytes);

        buffer(std::nullptr_t);

        std::type_info const &sample_type() const;
        std::size_t sample_byte_count() const;
        std::size_t size() const;
        void resize(std::size_t const);

        template <typename T>
        std::vector<T> const &vector() const;
        template <typename T>
        std::vector<T> &vector();

        template <typename T>
        T const *data() const;
        template <typename T>
        T *data();
    };

    template <typename T>
    processing::buffer make_buffer(std::size_t const size);
    template <typename T>
    processing::buffer make_buffer(std::size_t const size, std::size_t const reserve);
}
}

#include "yas_processing_buffer_private.h"
