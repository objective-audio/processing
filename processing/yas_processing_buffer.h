//
//  yas_processing_buffer.h
//

#pragma once

#include "yas_base.h"
#include <vector>

namespace yas {
namespace processing {
    class buffer : public base {
        class impl_base;

       public:
        template <typename T>
        class impl;

        template <typename T>
        buffer(std::vector<T> &&bytes);
        template <typename T>
        buffer(std::vector<T> &bytes);

        std::type_info const &sample_type() const;
        std::size_t sample_byte_count() const;
        std::size_t size() const;
    };

    template <typename T>
    processing::buffer make_buffer(std::size_t const);

    template <typename T>
    std::vector<T> const &get_vector(buffer const &);
    template <typename T>
    std::vector<T> &get_vector(buffer &);
}
}

#include "yas_processing_buffer_private.h"
