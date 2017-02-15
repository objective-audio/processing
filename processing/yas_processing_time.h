//
//  yas_processing_range.h
//

#pragma once

#include "yas_processing_types.h"
#include "yas_base.h"
#include <memory>
#include <vector>
#include <experimental/optional>

namespace yas {
namespace processing {
    class time : public base {
        template <typename T>
        class impl;

       public:
        class impl_base;

        struct frame {
            using type = frame_index_t;
        };

        struct any {
            using type = any;

            bool operator==(any const &) const;
            bool operator!=(any const &) const;
        };

        struct range {
            using type = range;

            frame_index_t frame = 0;
            length_t length = 0;

            range(frame_index_t const, length_t const);

            bool operator==(range const &) const;
            bool operator!=(range const &) const;
            bool operator<(range const &) const;

            frame_index_t next_frame() const;

            bool is_contain(range const &) const;
            bool is_contain(frame::type const &) const;
            bool is_contain(any::type const &) const;
            bool is_overlap(range const &) const;
            bool can_combine(range const &) const;
            std::experimental::optional<range> intersect(range const &) const;
            std::experimental::optional<range> combine(range const &) const;
            std::vector<range> crop(range const &) const;
            range offset(frame_index_t const &) const;
        };

        time(frame_index_t const, length_t const);
        explicit time(range);
        explicit time(frame_index_t const);
        time();
        time(std::nullptr_t);

        time &operator=(range const &);
        time &operator=(range &&);

        bool operator<(time const &) const;

        std::type_info const &type() const;
        bool is_range_type() const;
        bool is_frame_type() const;
        bool is_any_type() const;

        bool is_contain(time const &) const;

        template <typename T>
        typename T::type const &get() const;
        
        time offset(frame_index_t const &) const;

       private:
        static std::shared_ptr<processing::time::impl<time::any>> const &any_impl_ptr();
    };

    time make_range_time(frame_index_t const, length_t const);
    time make_frame_time(frame_index_t const);
    time make_any_time();
}
}
