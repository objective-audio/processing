//
//  yas_processing_track.h
//

#pragma once

#include "yas_base.h"
#include <map>

namespace yas {
namespace processing {
    class time_range;
    class module;
    class stream;

    class track : public base {
        class impl;

       public:
        track();
        track(std::nullptr_t);

        std::multimap<time_range, module> const &modules() const;
        std::multimap<time_range, module> &modules();

        void insert_module(time_range, module);

        void process(stream &);
    };
}
}
