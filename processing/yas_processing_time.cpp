//
//  yas_processing_time::range.cpp
//

#include "yas_processing_time.h"
#include <string>

using namespace yas;

#pragma mark - time::range

processing::time::range::range(frame_index_t const frame, length_t const length) : frame(frame), length(length) {
    if (this->length == 0) {
        throw "length is zero.";
    }
}

bool processing::time::range::operator==(time::range const &rhs) const {
    return this->frame == rhs.frame && this->length == rhs.length;
}

bool processing::time::range::operator!=(time::range const &rhs) const {
    return this->frame != rhs.frame || this->length != rhs.length;
}

bool processing::time::range::operator<(time::range const &rhs) const {
    if (this->frame != rhs.frame) {
        return this->frame < rhs.frame;
    }

    return this->length < rhs.length;
}

processing::frame_index_t processing::time::range::next_frame() const {
    return this->frame + this->length;
}

bool processing::time::range::is_contain(time::range const &rhs) const {
    if (this->length == 0) {
        return false;
    }

    if (this->frame > rhs.frame) {
        return false;
    }

    if (rhs.length > 0) {
        return rhs.next_frame() <= next_frame();
    } else {
        return rhs.frame < next_frame();
    }
}

bool processing::time::range::is_contain(frame::type const &rhs_frame) const {
    return this->frame <= rhs_frame && rhs_frame < next_frame();
}

bool processing::time::range::is_contain(any::type const &any) const {
    return true;
}

bool processing::time::range::is_overlap(time::range const &rhs) const {
    if (this->length == 0 || rhs.length == 0) {
        return false;
    }

    return std::max(this->frame, rhs.frame) < std::min(next_frame(), rhs.next_frame());
}

bool processing::time::range::can_combine(time::range const &rhs) const {
    if (this->length == 0 || rhs.length == 0) {
        return false;
    }

    bool const is_this_lower = this->frame <= rhs.frame;
    auto const &lower_range = is_this_lower ? *this : rhs;
    auto const &higher_range = is_this_lower ? rhs : *this;
    return lower_range.next_frame() >= higher_range.frame;
}

std::experimental::optional<processing::time::range> processing::time::range::intersect(time::range const &rhs) const {
    auto const start = std::max(this->frame, rhs.frame);
    auto const next = std::min(next_frame(), rhs.next_frame());

    if (start < next) {
        return time::range{start, static_cast<length_t>(next - start)};
    } else {
        return std::experimental::nullopt;
    }
}

std::experimental::optional<processing::time::range> processing::time::range::combine(time::range const &other) const {
    if (!can_combine(other)) {
        return std::experimental::nullopt;
    }

    auto const start = std::min(this->frame, other.frame);
    auto const next = std::max(next_frame(), other.next_frame());

    return time::range{start, static_cast<length_t>(next - start)};
}

std::vector<processing::time::range> processing::time::range::crop(range const &other) const {
    std::vector<processing::time::range> vec;

    if (!is_overlap(other)) {
        vec.push_back(*this);
        return vec;
    }

    if (auto const cropped_ragne_opt = intersect(other)) {
        auto const &cropped_range = *cropped_ragne_opt;
        if (this->frame < cropped_range.frame) {
            vec.emplace_back(time::range{this->frame, static_cast<length_t>(cropped_range.frame - this->frame)});
        }

        auto const cropped_next_frame = cropped_range.next_frame();
        auto const current_next_frame = next_frame();
        if (cropped_next_frame < current_next_frame) {
            vec.emplace_back(
                time::range{cropped_next_frame, static_cast<length_t>(current_next_frame - cropped_next_frame)});
        }
    }

    return vec;
}

processing::time::range processing::time::range::offset(frame_index_t const &offset) const {
    return time::range{this->frame + offset, this->length};
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
            auto const &type_info = typeid(T);
            if (type_info == casted_rhs->type()) {
                return this->_value == casted_rhs->_value;
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

processing::time::time(range range) : base(std::make_shared<impl<time::range>>(std::move(range))) {
}

processing::time::time(frame_index_t const frame) : base(std::make_shared<impl<time::frame>>(frame)) {
}

processing::time::time() : base(any_impl_ptr()) {
}

processing::time::time(std::nullptr_t) : base(nullptr) {
}

processing::time &processing::time::operator=(time::range const &range) {
    this->set_impl_ptr(std::make_shared<impl<time::range>>(range));
    return *this;
}

processing::time &processing::time::operator=(time::range &&range) {
    this->set_impl_ptr(std::make_shared<impl<time::range>>(std::move(range)));
    return *this;
}

bool processing::time::operator<(time const &rhs) const {
    if (this->is_any_type()) {
        if (rhs.is_any_type()) {
            return false;
        } else {
            return true;
        }
    } else if (rhs.is_any_type()) {
        return false;
    }

    if (this->is_frame_type()) {
        if (rhs.is_frame_type()) {
            return this->get<frame>() < rhs.get<frame>();
        } else if (rhs.is_range_type()) {
            return true;
        }
    } else if (this->is_range_type()) {
        if (rhs.is_frame_type()) {
            return false;
        } else if (rhs.is_range_type()) {
            return this->get<range>() < rhs.get<range>();
        }
    }

    throw "unreachable code.";
}

std::type_info const &processing::time::type() const {
    return this->impl_ptr<impl_base>()->type();
}

bool processing::time::is_range_type() const {
    return this->type() == typeid(range);
}

bool processing::time::is_frame_type() const {
    return this->type() == typeid(frame);
}

bool processing::time::is_any_type() const {
    return this->type() == typeid(any);
}

bool processing::time::is_contain(time const &rhs) const {
    if (this->is_range_type()) {
        auto const &range = this->get<time::range>();
        if (rhs.is_range_type()) {
            return range.is_contain(rhs.get<time::range>());
        } else if (rhs.is_frame_type()) {
            return range.is_contain(rhs.get<time::frame>());
        } else if (rhs.is_any_type()) {
            return range.is_contain(rhs.get<time::any>());
        }

        throw "unreachable code.";
    } else {
        return false;
    }
}

template <typename T>
typename T::type const &processing::time::get() const {
    if (auto ip = std::dynamic_pointer_cast<impl<T>>(this->impl_ptr())) {
        return ip->_value;
    }

    throw "unreachable code.";
}

template processing::time::range::type const &processing::time::get<processing::time::range>() const;
template processing::time::frame::type const &processing::time::get<processing::time::frame>() const;
template processing::time::any::type const &processing::time::get<processing::time::any>() const;

std::shared_ptr<processing::time::impl<processing::time::any>> const &processing::time::any_impl_ptr() {
    static auto impl_ptr = std::make_shared<time::impl<time::any>>(time::any{});
    return impl_ptr;
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
