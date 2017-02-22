//
//  yas_processing_compare_modules.cpp
//

#include "yas_processing_compare_modules.h"
#include "yas_processing_send_signal_processor.h"
#include "yas_processing_send_number_processor.h"
#include "yas_processing_receive_signal_processor.h"
#include "yas_processing_receive_number_processor.h"
#include "yas_processing_module.h"
#include "yas_processing_signal_event.h"
#include "yas_processing_constants.h"
#include "yas_fast_each.h"
#include "yas_boolean.h"

using namespace yas;

#pragma mark - signal

namespace yas {
namespace processing {
    namespace compare {
        struct signal_context {
            signal_event left_signal;
            signal_event right_signal;
            time left_time;
            time right_time;

            signal_context(signal_event &&left_signal, signal_event &&right_signal)
                : left_signal(std::move(left_signal)), right_signal(std::move(right_signal)) {
            }

            void reset(std::size_t const reserve_size) {
                this->left_signal.reserve(reserve_size);
                this->right_signal.reserve(reserve_size);
                this->left_signal.resize(0);
                this->right_signal.resize(0);
                this->left_time = nullptr;
                this->right_time = nullptr;
            }
        };
    }
}
}

template <typename T>
processing::module processing::make_signal_module(compare::kind const kind) {
    using namespace yas::processing::compare;

    auto context = std::make_shared<signal_context>(make_signal_event<T>(0), make_signal_event<T>(0));

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(stream.sync_source().slice_length); };

    auto receive_processor = processing::make_receive_signal_processor<T>(
        [context](time::range const &time_range, sync_source const &, channel_index_t const,
                  connector_index_t const co_idx, T const *const signal_ptr) mutable {
            if (co_idx == to_connector_index(input::left)) {
                context->left_time = time_range;
                context->left_signal.copy_from(signal_ptr, time_range.length);
            } else if (co_idx == to_connector_index(input::right)) {
                context->right_time = time_range;
                context->right_signal.copy_from(signal_ptr, time_range.length);
            }
        });

    auto send_processor = processing::make_send_signal_processor<boolean>(
        [context, kind](processing::time::range const &time_range, sync_source const &, channel_index_t const,
                        connector_index_t const co_idx, boolean *const signal_ptr) {
            if (co_idx == to_connector_index(output::result)) {
                auto out_each = make_fast_each(signal_ptr, time_range.length);
                processing::signal_event &left_signal = context->left_signal;
                processing::signal_event &right_signal = context->right_signal;
                auto const *left_ptr = left_signal.data<T>();
                auto const *right_ptr = right_signal.data<T>();
                processing::time const &left_time = context->left_time;
                processing::time const &right_time = context->right_time;
                auto const left_offset = left_time ? time_range.frame - left_time.get<time::range>().frame : 0;
                auto const right_offset = right_time ? time_range.frame - right_time.get<time::range>().frame : 0;
                auto const &left_length = left_time ? left_time.get<time::range>().length : constant::zero_length;
                auto const &right_length = right_time ? right_time.get<time::range>().length : constant::zero_length;

                while (yas_fast_each_next(out_each)) {
                    auto const &idx = yas_fast_each_index(out_each);
                    auto const left_idx = idx + left_offset;
                    auto const right_idx = idx + right_offset;
                    auto const &left_value = (left_idx >= 0 && left_idx < left_length) ? left_ptr[left_idx] : 0;
                    auto const &right_value = (right_idx >= 0 && right_idx < right_length) ? right_ptr[right_idx] : 0;

                    switch (kind) {
                        case kind::is_equal:
                            yas_fast_each_value(out_each) = left_value == right_value;
                            break;
                        case kind::is_not_equal:
                            yas_fast_each_value(out_each) = left_value != right_value;
                            break;

                        case kind::is_greater:
                            yas_fast_each_value(out_each) = left_value > right_value;
                            break;
                        case kind::is_greater_equal:
                            yas_fast_each_value(out_each) = left_value >= right_value;
                            break;
                        case kind::is_less:
                            yas_fast_each_value(out_each) = left_value < right_value;
                            break;
                        case kind::is_less_equal:
                            yas_fast_each_value(out_each) = left_value <= right_value;
                            break;
                    }
                }
            }
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_signal_module<double>(compare::kind const);
template processing::module processing::make_signal_module<float>(compare::kind const);
template processing::module processing::make_signal_module<int64_t>(compare::kind const);
template processing::module processing::make_signal_module<int32_t>(compare::kind const);
template processing::module processing::make_signal_module<int16_t>(compare::kind const);
template processing::module processing::make_signal_module<int8_t>(compare::kind const);
template processing::module processing::make_signal_module<uint64_t>(compare::kind const);
template processing::module processing::make_signal_module<uint32_t>(compare::kind const);
template processing::module processing::make_signal_module<uint16_t>(compare::kind const);
template processing::module processing::make_signal_module<uint8_t>(compare::kind const);
template processing::module processing::make_signal_module<boolean>(compare::kind const);

#pragma mark - number

namespace yas {
namespace processing {
    namespace compare {
        template <typename T>
        struct number_input {
            std::experimental::optional<T> left_value;
            std::experimental::optional<T> right_value;
        };

        template <typename T>
        struct number_context {
            std::map<time::frame::type, number_input<T>> inputs;
            T last_left_value = 0;
            T last_right_value = 0;

            void insert_input(frame_index_t const &frame, T const &value, compare::input const &direction) {
                if (this->inputs.count(frame) == 0) {
                    this->inputs.emplace(frame, number_input<T>{});
                }
                auto &input = this->inputs.at(frame);
                switch (direction) {
                    case input::left:
                        input.left_value = value;
                        break;
                    case input::right:
                        input.right_value = value;
                        break;
                }
            }

            void reset() {
                this->inputs.clear();
            }
        };
    }
}
}

template <typename T>
processing::module processing::make_number_module(compare::kind const kind) {
    using namespace yas::processing::compare;

    auto context = std::make_shared<number_context<T>>();

    auto prepare_processor = [context](time::range const &, connector_map_t const &, connector_map_t const &,
                                       stream &stream) mutable { context->reset(); };

    auto receive_processor =
        make_receive_number_processor<T>([context](processing::time::frame::type const &frame, channel_index_t const,
                                                   connector_index_t const co_idx, T const &value) mutable {
            if (co_idx == to_connector_index(input::left)) {
                context->insert_input(frame, value, input::left);
            } else if (co_idx == to_connector_index(input::right)) {
                context->insert_input(frame, value, input::right);
            }
        });

    auto send_processor = make_send_number_processor<boolean>(
        [context, kind](processing::time::range const &, sync_source const &, channel_index_t const,
                        connector_index_t const co_idx) mutable {
            number_event::value_map_t<boolean> result;

            if (co_idx == to_connector_index(output::result)) {
                for (auto const &input_pair : context->inputs) {
                    auto const &input = input_pair.second;
                    if (auto const &left_value = input.left_value) {
                        context->last_left_value = *left_value;
                    }
                    if (auto const &right_value = input.right_value) {
                        context->last_right_value = *right_value;
                    }

                    T const &left_value = context->last_left_value;
                    T const &right_value = context->last_right_value;
                    boolean result_value;

                    switch (kind) {
                        case kind::is_equal:
                            result_value = left_value == right_value;
                            break;
                        case kind::is_not_equal:
                            result_value = left_value != right_value;
                            break;

                        case kind::is_greater:
                            result_value = left_value > right_value;
                            break;
                        case kind::is_greater_equal:
                            result_value = left_value >= right_value;
                            break;
                        case kind::is_less:
                            result_value = left_value < right_value;
                            break;
                        case kind::is_less_equal:
                            result_value = left_value <= right_value;
                            break;
                    }

                    result.emplace(input_pair.first, result_value);
                }
            }

            return result;
        });

    return processing::module{{std::move(prepare_processor), std::move(receive_processor), std::move(send_processor)}};
}

template processing::module processing::make_number_module<double>(compare::kind const);
template processing::module processing::make_number_module<float>(compare::kind const);
template processing::module processing::make_number_module<int64_t>(compare::kind const);
template processing::module processing::make_number_module<int32_t>(compare::kind const);
template processing::module processing::make_number_module<int16_t>(compare::kind const);
template processing::module processing::make_number_module<int8_t>(compare::kind const);
template processing::module processing::make_number_module<uint64_t>(compare::kind const);
template processing::module processing::make_number_module<uint32_t>(compare::kind const);
template processing::module processing::make_number_module<uint16_t>(compare::kind const);
template processing::module processing::make_number_module<uint8_t>(compare::kind const);
template processing::module processing::make_number_module<boolean>(compare::kind const);
