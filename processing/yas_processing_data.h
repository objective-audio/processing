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
        std::size_t size() const;
    };

    template <typename T>
    processing::data make_data(std::size_t const);
    
    template <typename T>
    std::vector<T> const &get_raw(data const &);
    template <typename T>
    std::vector<T> &get_raw(data &);
}
}

#include "yas_processing_data_private.h"
