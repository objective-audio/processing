//
//  yas_processing_time_range.cpp
//

#include "yas_processing_time_range.h"
#include <string>

using namespace yas;

bool processing::time_range::operator==(time_range const &rhs) const {
    return start_frame == rhs.start_frame && length == rhs.length;
}

bool processing::time_range::operator!=(time_range const &rhs) const {
    return start_frame != rhs.start_frame || length != rhs.length;
}

bool processing::time_range::operator<(time_range const &rhs) const {
    if (start_frame != rhs.start_frame) {
        return start_frame < rhs.start_frame;
    }

    return length < rhs.length;
}

int64_t processing::time_range::next_frame() const {
    return start_frame + length;
}

bool processing::time_range::is_contain(time_range const &other) const {
    return start_frame <= other.start_frame && other.next_frame() <= next_frame();
}

bool processing::time_range::is_overlap(time_range const &other) const {
    return std::max(start_frame, other.start_frame) <= std::min(next_frame(), other.next_frame());
}

bool processing::time_range::can_combine(time_range const &other) const {
    bool const is_this_lower = start_frame <= other.start_frame;
    auto const &lower_range = is_this_lower ? *this : other;
    auto const &higher_range = is_this_lower ? other : *this;
    return lower_range.next_frame() >= higher_range.start_frame;
}

std::experimental::optional<processing::time_range> processing::time_range::intersect(time_range const &other) const {
    auto const start = std::max(start_frame, other.start_frame);
    auto const next = std::min(next_frame(), other.next_frame());
    
    if (start <= next) {
        return time_range{.start_frame = start, .length = static_cast<uint32_t>(next - start)};
    } else {
        return std::experimental::nullopt;
    }
}

std::experimental::optional<processing::time_range> processing::time_range::combine(time_range const &other) const {
    if (!can_combine(other)) {
        return std::experimental::nullopt;
    }
    
    auto const start = std::min(start_frame, other.start_frame);
    auto const next = std::max(next_frame(), other.next_frame());
    
    return time_range{.start_frame = start, .length = static_cast<uint32_t>(next - start)};
}
