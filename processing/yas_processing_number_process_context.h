//
//  yas_processing_number_process_context.h
//

#pragma once

#include <map>
#include <experimental/optional>

namespace yas {
namespace processing {
    template <typename T, std::size_t N>
    struct number_process_context {
        struct input {
            std::experimental::optional<T> values[N];
        };

        void insert_input(frame_index_t const &frame, T const &value, std::size_t const idx);
        void update_last_values(input const &input);
        void reset();

        std::map<frame_index_t, input> const &inputs();
        T const *last_values() const;

       private:
        std::map<frame_index_t, input> _inputs;
        T _last_values[N];
    };
}
}

#include "yas_processing_number_process_context_private.h"
