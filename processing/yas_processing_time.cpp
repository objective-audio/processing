//
//  yas_processing_time::range.cpp
//

#include "yas_processing_time.h"
#include <string>

using namespace yas;

#pragma mark - time::range

bool processing::time::range::operator==(time::range const &rhs) const {
    return frame == rhs.frame && length == rhs.length;
}

bool processing::time::range::operator!=(time::range const &rhs) const {
    return frame != rhs.frame || length != rhs.length;
}

bool processing::time::range::operator<(time::range const &rhs) const {
    if (frame != rhs.frame) {
        return frame < rhs.frame;
    }

    return length < rhs.length;
}

processing::frame_index_t processing::time::range::next_frame() const {
    return frame + length;
}

bool processing::time::range::is_contain(time::range const &other) const {
    if (length == 0) {
        return false;
    }

    if (frame > other.frame) {
        return false;
    }

    if (other.length > 0) {
        return other.next_frame() <= next_frame();
    } else {
        return other.frame < next_frame();
    }
}

bool processing::time::range::is_overlap(time::range const &other) const {
    if (length == 0 || other.length == 0) {
        return false;
    }

    return std::max(frame, other.frame) < std::min(next_frame(), other.next_frame());
}

bool processing::time::range::can_combine(time::range const &other) const {
    if (length == 0 || other.length == 0) {
        return false;
    }

    bool const is_this_lower = frame <= other.frame;
    auto const &lower_range = is_this_lower ? *this : other;
    auto const &higher_range = is_this_lower ? other : *this;
    return lower_range.next_frame() >= higher_range.frame;
}

std::experimental::optional<processing::time::range> processing::time::range::intersect(
    time::range const &other) const {
    auto const start = std::max(frame, other.frame);
    auto const next = std::min(next_frame(), other.next_frame());

    if (start <= next) {
        return time::range{.frame = start, .length = static_cast<length_t>(next - start)};
    } else {
        return std::experimental::nullopt;
    }
}

std::experimental::optional<processing::time::range> processing::time::range::combine(time::range const &other) const {
    if (!can_combine(other)) {
        return std::experimental::nullopt;
    }

    auto const start = std::min(frame, other.frame);
    auto const next = std::max(next_frame(), other.next_frame());

    return time::range{.frame = start, .length = static_cast<length_t>(next - start)};
}

#pragma mark - time::any

bool processing::time::any::operator==(time::any const &) const {
    return true;
}

bool processing::time::any::operator!=(time::any const &) const {
    return false;
}

#pragma mark - time::impl

struct processing::time::impl_base : base::impl {
    virtual std::type_info const &type() const = 0;
};

template <typename T>
struct processing::time::impl : impl_base {
    typename T::type _value;

    impl(typename T::type const &val) : _value(val) {
    }

    impl(typename T::type &&val) : _value(std::move(val)) {
    }

    virtual bool is_equal(std::shared_ptr<base::impl> const &rhs) const override {
        if (auto casted_rhs = std::dynamic_pointer_cast<impl>(rhs)) {
            auto &type_info = type();
            if (type_info == casted_rhs->type()) {
                return _value == casted_rhs->_value;
            }
        }

        return false;
    }

    std::type_info const &type() const override {
        return typeid(T);
    }
};

#pragma mark - processing::time

processing::time::time(frame_index_t const frame, length_t const length)
    : base(std::make_shared<impl<time::range>>(time::range{frame, length})) {
}

processing::time::time(frame_index_t const frame) : base(std::make_shared<impl<time::frame>>(frame)) {
}

processing::time::time() : base(std::make_shared<impl<time::any>>(any{})) {
}

processing::time::time(std::nullptr_t) : base(nullptr) {
}

#pragma mark - make

processing::time processing::make_range_time(frame_index_t const frame, length_t const length) {
    return processing::time{frame, length};
}

processing::time processing::make_frame_time(frame_index_t const frame) {
    return processing::time{frame};
}

processing::time processing::make_any_time() {
    return processing::time{};
}
