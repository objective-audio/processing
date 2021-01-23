//
//  yas_processing_range.h
//

#pragma once

#include <processing/yas_processing_common_types.h>

#include <memory>
#include <optional>
#include <vector>

namespace yas::proc {
struct time {
    template <typename T>
    class impl;

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
        std::optional<range> intersected(range const &) const;
        std::optional<range> combined(range const &) const;
        std::vector<range> cropped(range const &) const;
        range merged(range const &) const;
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

    explicit operator bool() const;
    bool operator==(time const &) const;
    bool operator!=(time const &) const;

   private:
    static std::shared_ptr<proc::time::impl<time::any>> const &any_impl_ptr();

    std::shared_ptr<impl_base> _impl;
};

time make_range_time(frame_index_t const, length_t const);
time make_frame_time(frame_index_t const);
time make_any_time();
}  // namespace yas::proc

namespace yas {
std::string to_string(proc::time const &);
std::string to_string(proc::time::range const &);
}  // namespace yas
