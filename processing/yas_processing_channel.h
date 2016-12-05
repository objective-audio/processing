//
//  yas_processing_channel.h
//

#pragma once

#include "yas_base.h"
#include <map>

namespace yas {
namespace processing {
    class time_range;
    class data;

    class channel : public base {
        class impl;

       public:
        channel();
        channel(std::nullptr_t);

        std::multimap<time_range, data> const &datas() const;
        std::multimap<time_range, data> &datas();

        void insert_data(time_range, data);
    };
}
}
