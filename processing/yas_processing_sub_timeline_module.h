//
//  yas_processing_timeline_module.h
//

#pragma once

#include "yas_processing_types.h"

namespace yas {
namespace processing {
    class module;
    class timeline;
    
    module make_module(timeline);
}
}
