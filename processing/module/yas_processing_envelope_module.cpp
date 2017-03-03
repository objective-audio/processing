//
//  yas_processing_envelope_module.cpp
//

#include "yas_processing_envelope_module.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_module.h"
#include "yas_fast_each.h"

using namespace yas;

namespace yas {
namespace processing {
    namespace envelope {
        template <typename T>
        struct context {
            context(anchors_t<T> &&anchors)
                : _anchors(std::move(anchors)),
                  _prev(_anchors.cbegin()),
                  _next(_anchors.cbegin()),
                  _end(_anchors.cend()) {
            }

            void reset(time::range const &current_range) {
                if (_last_range && _last_range->next_frame() != current_range.frame) {
                    _prev = _next = _anchors.cbegin();
                }

                _last_range = current_range;
            }

            T value(frame_index_t const env_frame) {
                if (_prev == _end) {
                    // もともと値がない
                    return 0;
                }

                if (_prev == _next) {
                    // 生成したばかり
                    ++_next;
                }

                if (env_frame < _prev->first) {
                    // 最初のanchorに到達していない
                    return _prev->second;
                }

                while (_next != _end && env_frame >= _next->first) {
                    _prev = _next;
                    ++_next;
                }

                if (_next == _end) {
                    // 最後まで到達している
                    return _prev->second;
                } else {
                    auto const &prev_value = _prev->second;
                    auto const &prev_frame = _prev->first;
                    double const rate = double(env_frame - prev_frame) / (_next->first - prev_frame);
                    return static_cast<T>((_next->second - prev_value) * rate + prev_value);
                }
            }

           private:
            anchors_t<T> _anchors;
            typename anchors_t<T>::const_iterator _prev;
            typename anchors_t<T>::const_iterator _next;
            typename anchors_t<T>::const_iterator _end;
            std::experimental::optional<time::range> _last_range;
        };
    }
}
}

template <typename T>
processing::module processing::envelope::make_signal_module(anchors_t<T> anchors, frame_index_t const offset) {
    auto context = std::make_shared<envelope::context<T>>(std::move(anchors));

    auto prepare_processor = [context](time::range const &current_range, connector_map_t const &,
                                       connector_map_t const &, stream &) mutable { context->reset(current_range); };

    auto send_processor = processing::make_send_signal_processor<T>([context, offset](
        processing::time::range const &time_range, sync_source const &sync_src, channel_index_t const,
        connector_index_t const co_idx, T *const signal_ptr) {
        static auto const output_co_idx = to_connector_index(output::value);
        if (co_idx == output_co_idx) {
            auto out_each = make_fast_each(signal_ptr, time_range.length);

            while (yas_fast_each_next(out_each)) {
                auto const &idx = yas_fast_each_index(out_each);
                auto const env_idx = time_range.frame + idx - offset;
                yas_fast_each_value(out_each) = context->value(env_idx);
            }
        }
    });

    return processing::module{{std::move(prepare_processor), std::move(send_processor)}};
}

template processing::module processing::envelope::make_signal_module(anchors_t<double>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<float>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<int64_t>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<int32_t>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<int16_t>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<int8_t>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<uint64_t>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<uint32_t>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<uint16_t>, frame_index_t const);
template processing::module processing::envelope::make_signal_module(anchors_t<uint8_t>, frame_index_t const);

void yas::connect(processing::module &module, processing::envelope::output const &output,
                  processing::channel_index_t const &ch_idx) {
    module.connect_output(processing::to_connector_index(output), ch_idx);
}

std::string yas::to_string(processing::envelope::output const &output) {
    using namespace yas::processing::envelope;

    switch (output) {
        case output::value:
            return "value";
    }

    throw "output not found.";
}
