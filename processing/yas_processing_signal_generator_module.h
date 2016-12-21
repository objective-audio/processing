
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
        static auto constexpr result_key = "result";
        
        template <typename T>
        class impl;

        template <typename T>
        using process_f = std::function<void(processing::time_range const &, T *)>;

        explicit signal_generator_module(std::shared_ptr<signal_generator_module::impl_base> &&);
        signal_generator_module(std::nullptr_t);
    };

    template <typename T>
    signal_generator_module make_signal_generator_module(processing::time_range, signal_generator_module::process_f<T>);
}
}

#include "yas_processing_signal_generator_module_private.h"
