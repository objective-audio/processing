//
//  yas_processing_track.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time_range.h"
#include <map>

namespace yas {
namespace processing {
    class module;
    class stream;

    class track : public base {
        class impl;

       public:
        track();
        track(std::nullptr_t);

        std::multimap<time::range, module> const &modules() const;
        std::multimap<time::range, module> &modules();

        void insert_module(time::range, module);

        void process(time::range const &, stream &);
    };
}
}
