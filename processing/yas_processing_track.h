//
//  yas_processing_track.h
//

#pragma once

#include <map>
#include <optional>
#include "yas_base.h"
#include "yas_processing_time.h"

namespace yas::proc {
class module;
class stream;

class track : public base {
    class impl;

   public:
    track();
    track(std::nullptr_t);

    std::multimap<time::range, module> const &modules() const;
    std::multimap<time::range, module> &modules();

    std::optional<time::range> total_range() const;

    void insert_module(time::range, module);
    void remove_module(module const &);

    void process(time::range const &, stream &);
};
}  // namespace yas::proc
