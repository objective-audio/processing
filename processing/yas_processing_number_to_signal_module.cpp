//
//  yas_processing_number_to_signal_module.cpp
//

#include "yas_processing_number_to_signal_module.h"
#include "yas_processing_module.h"
#include "yas_processing_number_event.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_receive_number_processor.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_remove_number_processor.h"
#include "yas_fast_each.h"
#include <map>

using namespace yas;

namespace yas {
namespace processing {
    namespace number_to_signal {
        template <typename T>
        struct context {
            std::map<time::frame::type, T> number_events;
            T last_value = 0;

            void reset() {
                this->number_events.clear();
            }
        };

        template <typename T>
        using context_sptr = std::shared_ptr<context<T>>;

        template <typename T>
        context_sptr<T> make_context() {
            return std::make_shared<context<T>>();
        }
    }
}
}

template <typename T>
processing::module processing::make_number_to_signal_module() {
    auto context = number_to_signal::make_context<T>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &) mutable { context->reset(); };

    auto receive_processor =
        make_receive_number_processor<T>([context](processing::time::frame::type const &frame, channel_index_t const,
                                                   connector_index_t const, T const &value) mutable {
            auto &number_events = context->number_events;
            if (number_events.count(frame) > 0) {
                number_events.erase(frame);
            }
            context->number_events.emplace(std::make_pair(frame, value));
        });

    auto remove_processor = make_remove_number_processor<T>({to_connector_index(number_to_signal::input_key::number)});

    auto send_processor = make_send_signal_processor<T>(
        [context](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                  connector_index_t const con_idx, T *const signal_ptr) {
            auto const top_frame = time_range.frame;
            auto iterator = context->number_events.begin();
            auto const end_iterator = context->number_events.end();
            auto &last_value = context->last_value;

            auto out_each = make_fast_each(signal_ptr, time_range.length);
            while (yas_fast_each_next(out_each)) {
                auto const frame = top_frame + yas_fast_each_index(out_each);
                if (iterator != end_iterator) {
                    if ((*iterator).first == frame) {
                        last_value = (*iterator).second;
                        ++iterator;
                    }
                }
                yas_fast_each_value(out_each) = last_value;
            }
        });

    return module{{std::move(prepare_processor), std::move(receive_processor), std::move(remove_processor),
                   std::move(send_processor)}};
}

template processing::module processing::make_number_to_signal_module<double>();
template processing::module processing::make_number_to_signal_module<float>();
template processing::module processing::make_number_to_signal_module<int64_t>();
template processing::module processing::make_number_to_signal_module<int32_t>();
template processing::module processing::make_number_to_signal_module<int16_t>();
template processing::module processing::make_number_to_signal_module<int8_t>();
template processing::module processing::make_number_to_signal_module<uint64_t>();
template processing::module processing::make_number_to_signal_module<uint32_t>();
template processing::module processing::make_number_to_signal_module<uint16_t>();
template processing::module processing::make_number_to_signal_module<uint8_t>();
