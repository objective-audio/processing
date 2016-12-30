//
//  yas_processing_processor.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time_range.h"
#include "yas_processing_connector.h"

namespace yas {
namespace processing {
    class module;
    class stream;

    class processor : public base {
       public:
        struct impl : base::impl {
            virtual void process(module const &, time_range const &, stream &) = 0;
        };

        explicit processor(std::shared_ptr<impl> &&);
        processor(std::nullptr_t);

        void process(module const &, time_range const &, stream &);
    };

    template <typename T>
    using send_signal_f = std::function<void(processing::time_range const &, connector::channel_index_t const,
                                             std::string const &, T *const)>;

    template <typename T>
    using receive_signal_f = std::function<void(processing::time_range const &, connector::channel_index_t const,
                                                std::string const &, T const *const)>;

    template <typename T>
    processor make_send_signal_processor(send_signal_f<T>);

    template <typename T>
    processor make_receive_signal_processor(receive_signal_f<T>);
}
}
