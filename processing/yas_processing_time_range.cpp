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
