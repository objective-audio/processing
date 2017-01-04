//
//  yas_processing_time::range.cpp
//

#include "yas_processing_time_range.h"
#include <string>

using namespace yas;

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
