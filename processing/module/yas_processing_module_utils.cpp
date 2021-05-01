//
//  yas_processing_module_utils.cpp
//

#include "yas_processing_module_utils.h"

using namespace yas;
using namespace yas::proc;

frame_index_t proc::module_frame(frame_index_t const &time_frame, frame_index_t const offset) {
    return time_frame - offset;
}
