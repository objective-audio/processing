//
//  yas_processing_timeline_module.cpp
//

#include "yas_processing_sub_timeline_module.h"
#include "yas_processing_module.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_timeline.h"

using namespace yas;

namespace yas::proc {
namespace sub_timeline {
    struct context {
        timeline_ptr const timeline;

        context(proc::timeline_ptr const &timeline) : timeline(timeline) {
        }
    };

    std::shared_ptr<context> make_context(timeline_ptr const &timeline) {
        return std::make_shared<context>(timeline);
    }
}  // namespace sub_timeline
}  // namespace yas::proc

proc::module proc::make_module(timeline_ptr const &timeline, frame_index_t const offset) {
    auto context = sub_timeline::make_context(timeline);

    auto make_processors = [context = std::move(context), offset] {
        auto processor = [context, offset](time::range const &time_range, connector_map_t const &input_connectors,
                                           connector_map_t const &output_connectors, stream &stream) mutable {
            proc::stream sub_stream{stream.sync_source()};

            for (auto const &connector : input_connectors) {
                auto const &ch_idx = connector.second.channel_index;

                if (stream.has_channel(ch_idx)) {
                    auto const &input_channel = stream.channel(ch_idx);
                    auto const &co_idx = connector.first;

                    if (sub_stream.has_channel(co_idx)) {
                        throw "channel already exists in sub_stream.";
                    }

                    sub_stream.add_channel(co_idx, input_channel.copied_events(time_range, -offset));
                }
            }

            context->timeline->process(time_range.offset(-offset), sub_stream);

            for (auto const &connector : output_connectors) {
                auto const &co_idx = connector.first;

                if (sub_stream.has_channel(co_idx)) {
                    auto const &ch_idx = connector.second.channel_index;
                    auto &out_channel = stream.add_channel(ch_idx);
                    auto const &sub_channel = sub_stream.channel(co_idx);

                    out_channel.erase_events(time_range);

                    for (auto &event_pair : sub_channel.events()) {
                        auto const &time = event_pair.first;
                        auto const &event = event_pair.second;
                        if (time.is_range_type()) {
                            if (auto signal_event = std::dynamic_pointer_cast<proc::signal_event>(event)) {
                                out_channel.combine_signal_event(time.get<time::range>().offset(offset), signal_event);
                            }
                        } else {
                            out_channel.insert_event(time.offset(offset), event);
                        }
                    }
                }
            }
        };

        return module::processors_t{std::move(processor)};
    };

    return proc::module{std::move(make_processors)};
}
