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
    using modules_map_t = std::multimap<time::range, module>;

    track();
    track(std::nullptr_t);

    modules_map_t const &modules() const;
    modules_map_t &modules();

    std::optional<time::range> total_range() const;

    void insert_module(time::range, module);
    void remove_module(module const &);

    void process(time::range const &, stream &);
};
}  // namespace yas::proc
