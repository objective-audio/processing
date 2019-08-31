//
//  yas_processing_ptr.h
//

#pragma once

#include <memory>

namespace yas::proc {
class track;
class timeline;
class module;
class stream;
class sync_source;

using track_ptr = std::shared_ptr<track>;
using timeline_ptr = std::shared_ptr<timeline>;
using module_ptr = std::shared_ptr<module>;
}  // namespace yas::proc
