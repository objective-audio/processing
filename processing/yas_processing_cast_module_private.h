//
//  yas_processing_cast_module_private.h
//

#pragma once

#include "yas_processing_send_signal_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_receive_number_processor.h"
#include "yas_processing_remove_signal_processor.h"
#include "yas_processing_remove_number_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_channel.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_number_event.h"
#include "yas_fast_each.h"

namespace yas {
namespace processing {
    namespace cast {
        struct signal_context {
            signal_event signal;
            time time;

            signal_context(signal_event &&);

            void reset();
        };

        using signal_context_sptr = std::shared_ptr<signal_context>;

        template <typename T>
        signal_context_sptr make_signal_context();

        template <typename In, typename Out>
        processing::module make_signal_module() {
            auto context = make_signal_context<In>();

            auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                               stream &) mutable { context->reset(); };

            auto receive_processor = processing::make_receive_signal_processor<In>(
                [context](processing::time::range const &time_range, channel_index_t const, std::string const &key,
                          In const *const signal_ptr) mutable {
                    if (key == in_connector_key) {
                        context->time = time_range;
                        context->signal.copy_from(signal_ptr, time_range.length);
                    }
                });

            auto remove_processor = processing::make_remove_signal_processor<In>();

            auto send_processor = processing::make_send_signal_processor<Out>(
                [context](processing::time::range const &time_range, channel_index_t const, std::string const &key,
                          Out *const signal_ptr) {
                    if (key == out_connector_key) {
                        auto out_each = make_fast_each(signal_ptr, time_range.length);
                        processing::signal_event const &signal = context->signal;
                        auto const *src_ptr = signal.data<In>();
                        processing::time const &src_time = context->time;
                        auto const src_offset = src_time ? time_range.frame - src_time.get<time::range>().frame : 0;
                        auto const &src_length = src_time ? src_time.get<time::range>().length : 0;

                        while (yas_fast_each_next(out_each)) {
                            auto const &idx = yas_fast_each_index(out_each);
                            auto const src_idx = idx + src_offset;
                            auto const &src_value = (src_idx >= 0 && src_idx < src_length) ? src_ptr[src_idx] : 0;
                            yas_fast_each_value(out_each) = static_cast<Out>(src_value);
                        }
                    }
                });

            return processing::module{{std::move(prepare_processor), std::move(receive_processor),
                                       std::move(remove_processor), std::move(send_processor)}};
        }

        template <typename T>
        struct number_context {
            std::multimap<frame_index_t, T> numbers;

            void reset();
        };

        template <typename T>
        using number_context_sptr = std::shared_ptr<number_context<T>>;

        template <typename T>
        number_context_sptr<T> make_number_context();

        template <typename In, typename Out>
        processing::module make_number_module() {
            auto context = make_number_context<In>();

            auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                               stream &) mutable { context->reset(); };

            auto receive_processor = processing::make_receive_number_processor<In>(
                [context](processing::time::frame::type const &frame, channel_index_t const ch_idx,
                          std::string const &key, In const &value) {
                    if (key == in_connector_key) {
                        context->numbers.emplace(frame, value);
                    }
                });

            auto remove_processor = processing::make_remove_number_processor<In>();

            auto send_processor = [context](time::range const &current_time_range, connector_map_t const &,
                                            connector_map_t const &output_connectors, stream &stream) {
                if (output_connectors.count(out_connector_key) > 0) {
                    auto const &connector = output_connectors.at(out_connector_key);
                    auto const &ch_idx = connector.channel_index;
                    auto &channel = stream.has_channel(ch_idx) ? stream.channel(ch_idx) : stream.add_channel(ch_idx);
                    for (auto const &pair : context->numbers) {
                        channel.insert_event(make_frame_time(pair.first), number_event{static_cast<Out>(pair.second)});
                    }
                }
            };

            return processing::module{{std::move(prepare_processor), std::move(receive_processor),
                                       std::move(remove_processor), std::move(send_processor)}};
        }
    }
}
}
