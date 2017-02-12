//
//  yas_processing_timeline_module.cpp
//

#include "yas_processing_sub_timeline_module.h"
#include "yas_processing_module.h"
#include "yas_processing_timeline.h"
#include "yas_processing_module.h"
#include "yas_processing_sync_source.h"
#include "yas_processing_signal_event.h"

using namespace yas;

namespace yas {
namespace processing {
    namespace sub_timeline {
        struct context {
            timeline timeline;

            context(processing::timeline &&timeline) : timeline(std::move(timeline)) {
            }
        };

        using context_sptr = std::shared_ptr<context>;

        context_sptr make_context(timeline &&timeline) {
            return std::make_shared<context>(std::move(timeline));
        }
    }
}
}

processing::module processing::make_module(processing::timeline timeline) {
    auto context = sub_timeline::make_context(std::move(timeline));

    auto processor = [context](time::range const &time_range, connector_map_t const &input_connectors,
                               connector_map_t const &output_connectors, stream &stream) mutable {
        processing::stream sub_stream{stream.sync_source()};

        for (auto const &connector : input_connectors) {
            auto const &ch_idx = connector.second.channel_index;

            if (stream.has_channel(ch_idx)) {
                auto const &input_channel = stream.channel(ch_idx);
                auto const &con_idx = connector.first;

                if (sub_stream.has_channel(con_idx)) {
                    throw "channel already exists in sub_stream.";
                }

                sub_stream.add_channel(con_idx, input_channel.copied_events(time_range));
            }
        }

        context->timeline.process(time_range, sub_stream);

        for (auto const &connector : output_connectors) {
            auto const &con_idx = connector.first;

            if (sub_stream.has_channel(con_idx)) {
                auto const &ch_idx = connector.second.channel_index;
                auto &out_channel = stream.add_channel(ch_idx);
                auto const &sub_channel = sub_stream.channel(con_idx);

                out_channel.erase_events(time_range);
                out_channel.insert_events(sub_channel.events());
            }
        }
    };

    return processing::module{{std::move(processor)}};
}
