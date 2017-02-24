//
//  yas_processing_routing_module.cpp
//

#include "yas_processing_routing_modules.h"
#include "yas_processing_module.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_remove_signal_processor.h"
#include "yas_fast_each.h"
#include "yas_boolean.h"

using namespace yas;

namespace yas {
namespace processing {
    namespace routing {
        struct context {
            signal_event signal;
            time time;

            context(signal_event &&signal) : signal(std::move(signal)) {
            }

            void reset(std::size_t const reserve_size) {
                this->signal.reserve(reserve_size);
                this->signal.resize(0);
                this->time = nullptr;
            }
        };

        using context_sptr = std::shared_ptr<context>;

        template <typename T>
        context_sptr make_context() {
            return std::make_shared<context>(make_signal_event<T>(0));
        }
    }
}
}

template <typename T>
processing::module processing::make_signal_module(processing::routing::kind const kind) {
    using namespace yas::processing::routing;

    auto context = make_context<T>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(stream.sync_source().slice_length); };

    auto receive_processor = processing::make_receive_signal_processor<T>(
        [context](time::range const &time_range, sync_source const &, channel_index_t const,
                  connector_index_t const co_idx, T const *const signal_ptr) mutable {
            if (co_idx == to_connector_index(routing::input::value)) {
                context->time = time_range;
                context->signal.copy_from(signal_ptr, time_range.length);
            }
        });

    auto remove_processor = processing::make_remove_signal_processor<T>({to_connector_index(routing::input::value)});

    auto send_processor = processing::make_send_signal_processor<T>(
        [context, kind](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                        connector_index_t const co_idx, T *const signal_ptr) {
            if (co_idx == to_connector_index(routing::output::value)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                processing::signal_event const &input_signal = context->signal;
                auto const *src_ptr = input_signal.data<T>();
                processing::time const &input_time = context->time;
                auto const src_offset = input_time ? time_range.frame - input_time.get<time::range>().frame : 0;
                auto const &src_length = input_time ? input_time.get<time::range>().length : 0;

                while (yas_fast_each_next(out_each)) {
                    auto const &idx = yas_fast_each_index(out_each);
                    auto const src_idx = idx + src_offset;
                    auto const &src_value = (src_idx >= 0 && src_idx < src_length) ? src_ptr[src_idx] : 0;
                    yas_fast_each_value(out_each) = static_cast<T>(src_value);
                }
            }
        });

    module::processors_t processors{prepare_processor, receive_processor};
    if (kind == routing::kind::move) {
        processors.emplace_back(std::move(remove_processor));
    }
    processors.emplace_back(std::move(send_processor));

    return processing::module{std::move(processors)};
}

template processing::module processing::make_signal_module<double>(processing::routing::kind const);
template processing::module processing::make_signal_module<float>(processing::routing::kind const);
template processing::module processing::make_signal_module<int64_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<int32_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<int16_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<int8_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<uint64_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<uint32_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<uint16_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<uint8_t>(processing::routing::kind const);
template processing::module processing::make_signal_module<boolean>(processing::routing::kind const);
