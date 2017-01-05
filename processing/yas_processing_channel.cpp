//
//  yas_processing_channel.cpp
//

#include "yas_processing_channel.h"
#include "yas_processing_buffer.h"

using namespace yas;

#pragma mark - processing::channel::impl

struct processing::channel::impl : base::impl {
    std::multimap<time, buffer> _buffers;
};

#pragma mark - processing::channel

processing::channel::channel() : base(std::make_shared<impl>()) {
}

processing::channel::channel(std::nullptr_t) : base(nullptr) {
}

std::multimap<processing::time, processing::buffer> const &processing::channel::buffers() const {
    return impl_ptr<impl>()->_buffers;
}

std::multimap<processing::time, processing::buffer> &processing::channel::buffers() {
    return impl_ptr<impl>()->_buffers;
}

void processing::channel::insert_buffer(time time, buffer buffer) {
    if (time.type() == typeid(time::range)) {
        auto const &time_range = time.get<time::range>();
        if (time_range.length > 0 && time_range.length != buffer.size()) {
            throw "invalid buffer size.";
        }
    } else {
        if (buffer.size() != 1) {
            throw "invalid buffer size.";
        }
    }

    impl_ptr<impl>()->_buffers.emplace(std::move(time), std::move(buffer));
}
