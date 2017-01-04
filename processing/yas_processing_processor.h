//
//  yas_processing_processor.h
//

#pragma once

#include "yas_base.h"
#include "yas_processing_time_range.h"
#include "yas_processing_types.h"
#include "yas_processing_connector.h"

namespace yas {
namespace processing {
    class module;
    class stream;

    class processor : public base {
       public:
        struct impl : base::impl {
            virtual void process(time::range const &, connector_map_t const &inputs, connector_map_t const &outputs,
                                 stream &) = 0;
        };

        explicit processor(std::shared_ptr<impl> &&);
        processor(std::nullptr_t);

        void process(time::range const &, connector_map_t const &inputs, connector_map_t const &outputs, stream &);
    };
}
}
