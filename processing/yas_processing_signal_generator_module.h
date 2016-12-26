//
//  yas_processing_signal_generator_module.h
//

#pragma once

#include "yas_processing_module.h"

namespace yas {
namespace processing {
    class signal_generator_module : public module {
        class impl_base;

       public:
        template <typename T>
        class impl;

        template <typename T>
        using send_signal_f = std::function<void(processing::time_range const &, int64_t const, std::string const &, T *const)>;
        template <typename T>
        using receive_signal_f = std::function<void(processing::time_range const &, int64_t const, std::string const &, T const *const)>;

        template <typename T>
        struct args {
            processing::time_range time_range;
            send_signal_f<T> send_signal_handler;
            receive_signal_f<T> receive_signal_handler;
        };

        explicit signal_generator_module(std::shared_ptr<signal_generator_module::impl_base> &&);
        signal_generator_module(std::nullptr_t);
    };

    template <typename T>
    signal_generator_module make_signal_generator_module(signal_generator_module::args<T>);
}
}

#include "yas_processing_signal_generator_module_private.h"
