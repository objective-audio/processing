//
//  yas_processing_data.h
//

#pragma once

#include "yas_base.h"
#include <vector>

namespace yas {
namespace processing {
    class data : public base {
        class impl_base;

       public:
        template <typename T>
        class impl;

        template <typename T>
        data(std::vector<T> &&bytes);
        template <typename T>
        data(std::vector<T> &bytes);

        std::type_info const &sample_type() const;
        std::size_t sample_byte_count() const;

        template <typename T>
        std::vector<T> const &raw() const;
        template <typename T>
        std::vector<T> &raw();
    };

    template <typename T>
    processing::data make_data(std::size_t const);
}
}

#include "yas_processing_data_private.h"
