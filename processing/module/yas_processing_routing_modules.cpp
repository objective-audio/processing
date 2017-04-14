//
//  yas_processing_routing_module.cpp
//

#include "yas_processing_routing_modules.h"
#include "yas_processing_module.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_remove_signal_processor.h"
#include "yas_processing_signal_process_context.h"
#include "yas_processing_receive_number_processor.h"
#include "yas_processing_send_number_processor.h"
#include "yas_processing_remove_number_processor.h"
#include "yas_processing_number_process_context.h"
#include "yas_fast_each.h"
#include "yas_boolean.h"

using namespace yas;

#pragma mark - signal

template <typename T>
processing::module processing::make_signal_module(processing::routing::kind const kind) {
    using namespace yas::processing::routing;

    auto context = std::make_shared<signal_process_context<T, 1>>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(stream.sync_source().slice_length); };

    auto receive_processor = processing::make_receive_signal_processor<T>(
        [context](time::range const &time_range, sync_source const &, channel_index_t const,
                  connector_index_t const co_idx, T const *const signal_ptr) mutable {
            if (co_idx == to_connector_index(input::value)) {
                context->set_time(time{time_range}, co_idx);
                context->copy_data_from(signal_ptr, time_range.length, co_idx);
            }
        });

    auto send_processor = processing::make_send_signal_processor<T>(
        [context, kind](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                        connector_index_t const co_idx, T *const signal_ptr) {
            if (co_idx == to_connector_index(output::value)) {
                static auto const input_co_idx = to_connector_index(input::value);

                auto const *src_ptr = context->data(input_co_idx);
                processing::time const &input_time = context->time(input_co_idx);
                auto const src_offset = input_time ? time_range.frame - input_time.get<time::range>().frame : 0;
                auto const &src_length = input_time ? input_time.get<time::range>().length : 0;

                auto out_each = make_fast_each(signal_ptr, time_range.length);
                while (yas_each_next(out_each)) {
                    auto const &idx = yas_each_index(out_each);
                    auto const src_idx = idx + src_offset;
                    auto const &src_value = (src_idx >= 0 && src_idx < src_length) ? src_ptr[src_idx] : 0;
                    yas_each_value(out_each) = src_value;
                }
            }
        });

    module::processors_t processors{prepare_processor, receive_processor};
    if (kind == kind::move) {
        auto remove_processor = processing::make_remove_signal_processor<T>({to_connector_index(input::value)});
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

#pragma mark - number

template <typename T>
processing::module processing::make_number_module(routing::kind const kind) {
    using namespace yas::processing::routing;

    auto context = std::make_shared<number_process_context<T, 1>>();

    auto prepare_processor = [context](time::range const &current_range, connector_map_t const &,
                                       connector_map_t const &,
                                       stream &stream) mutable { context->reset(current_range); };

    auto receive_processor =
        make_receive_number_processor<T>([context](processing::time::frame::type const &frame, channel_index_t const,
                                                   connector_index_t const co_idx, T const &value) mutable {
            if (co_idx == to_connector_index(input::value)) {
                context->insert_input(frame, value, 0);
            }
        });

    auto send_processor =
        make_send_number_processor<T>([context, kind](processing::time::range const &, sync_source const &,
                                                      channel_index_t const, connector_index_t const co_idx) mutable {
            number_event::value_map_t<T> result;

            if (co_idx == to_connector_index(output::value)) {
                static auto const input_co_idx = to_connector_index(input::value);

                for (auto const &input_pair : context->inputs()) {
                    auto const &input_value = *input_pair.second.values[input_co_idx];
                    result.emplace(input_pair.first, input_value);
                }
            }

            return result;
        });

    module::processors_t processors{prepare_processor, receive_processor};
    if (kind == kind::move) {
        auto remove_processor = processing::make_remove_number_processor<T>({to_connector_index(input::value)});
        processors.emplace_back(std::move(remove_processor));
    }
    processors.emplace_back(std::move(send_processor));

    return processing::module{std::move(processors)};
}

template processing::module processing::make_number_module<double>(processing::routing::kind const);
template processing::module processing::make_number_module<float>(processing::routing::kind const);
template processing::module processing::make_number_module<int64_t>(processing::routing::kind const);
template processing::module processing::make_number_module<int32_t>(processing::routing::kind const);
template processing::module processing::make_number_module<int16_t>(processing::routing::kind const);
template processing::module processing::make_number_module<int8_t>(processing::routing::kind const);
template processing::module processing::make_number_module<uint64_t>(processing::routing::kind const);
template processing::module processing::make_number_module<uint32_t>(processing::routing::kind const);
template processing::module processing::make_number_module<uint16_t>(processing::routing::kind const);
template processing::module processing::make_number_module<uint8_t>(processing::routing::kind const);
template processing::module processing::make_number_module<boolean>(processing::routing::kind const);

#pragma mark -

void yas::connect(processing::module &module, processing::routing::input const &input,
                  processing::channel_index_t const &ch_idx) {
    module.connect_input(processing::to_connector_index(input), ch_idx);
}

void yas::connect(processing::module &module, processing::routing::output const &output,
                  processing::channel_index_t const &ch_idx) {
    module.connect_output(processing::to_connector_index(output), ch_idx);
}

std::string yas::to_string(processing::routing::input const &input) {
    using namespace yas::processing::routing;

    switch (input) {
        case input::value:
            return "value";
    }

    throw "input not found.";
}

std::string yas::to_string(processing::routing::output const &output) {
    using namespace yas::processing::routing;

    switch (output) {
        case output::value:
            return "value";
    }

    throw "output not found.";
}
