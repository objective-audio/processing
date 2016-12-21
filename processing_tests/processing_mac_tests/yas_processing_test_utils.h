//
//  yas_processing_test_utils.h
//

#include "yas_processing.h"
#include <functional>

#pragma once

namespace yas {
namespace test {
    struct module : processing::module {
        using process_f = std::function<void(processing::stream &)>;

        struct impl : processing::module::impl {
            int64_t _value = 0;
            int64_t _frame_offset = 0;
            process_f _process_handler;

            impl(module::impl::args args) : processing::module::impl(std::move(args)) {
            }

            void process(processing::stream &stream) override {
                if (_process_handler) {
                    _process_handler(stream);
                }
            }

            int64_t frame_offset() override {
                return _frame_offset;
            }
        };

        module() : processing::module(std::make_shared<impl>(module::impl::args{})) {
        }

        module(std::nullptr_t) : processing::module(nullptr) {
        }

        void set_value(int64_t const value) {
            impl_ptr<impl>()->_value = value;
        }

        int64_t value() const {
            return impl_ptr<impl>()->_value;
        }

        void set_frame_offset(int64_t const offset) {
            impl_ptr<impl>()->_frame_offset = offset;
        }

        void set_process_handler(process_f handler) {
            impl_ptr<impl>()->_process_handler = std::move(handler);
        }
    };
/*
    struct signal_generator_module : processing::signal_generator_module {
        using process_f = std::function<void(processing::time_range const &, double *)>;

        struct impl : processing::signal_generator_module::impl<double> {
            process_f _process_handler;

            impl(processing::time_range time_range)
                : processing::signal_generator_module::impl<double>(std::move(time_range)) {
            }

            void process_signal(processing::time_range const &time_range, double *signal_ptr) override {
                if (_process_handler) {
                    _process_handler(time_range, signal_ptr);
                }
            }
        };

        signal_generator_module(processing::time_range time_range)
            : processing::signal_generator_module(std::make_shared<impl>(std::move(time_range))) {
        }
        
        void set_process_handler(process_f handler) {
            impl_ptr<impl>()->_process_handler = std::move(handler);
        }
    };
 */
}
}
