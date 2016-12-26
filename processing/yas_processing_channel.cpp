//
//  yas_processing_channel.cpp
//

#include "yas_processing_channel.h"
#include "yas_processing_data.h"
#include "yas_processing_time_range.h"

using namespace yas;

#pragma mark - processing::channel::impl

struct processing::channel::impl : base::impl {
    std::multimap<time_range, data> _datas;
};

#pragma mark - processing::channel

processing::channel::channel() : base(std::make_shared<impl>()) {
}

processing::channel::channel(std::nullptr_t) : base(nullptr) {
}

std::multimap<processing::time_range, processing::data> const &processing::channel::datas() const {
    return impl_ptr<impl>()->_datas;
}

std::multimap<processing::time_range, processing::data> &processing::channel::datas() {
    return impl_ptr<impl>()->_datas;
}

void processing::channel::insert_data(time_range time_range, data data) {
    if (time_range.length > 0 && time_range.length != data.size()) {
        throw "invalid data size.";
    }

    impl_ptr<impl>()->_datas.emplace(std::move(time_range), std::move(data));
}
