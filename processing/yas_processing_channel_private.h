//
//  yas_processing_channel_private.h
//

#pragma once

namespace yas {
template <typename P>
void processing::channel::erase_data_if(P predicate) {
    erase_if(this->datas(), predicate);
}
}
