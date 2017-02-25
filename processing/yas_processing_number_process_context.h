//
//  yas_processing_number_process_context.h
//

#pragma once

#include "yas_processing_types.h"
#include <map>
#include <vector>
#include <experimental/optional>

namespace yas {
namespace processing {
    template <typename T, std::size_t N>
    struct number_process_context {
        struct input {
            std::experimental::optional<T> values[N];
        };

        number_process_context();

        void insert_input(frame_index_t const &frame, T const &value, std::size_t const idx);
        void update_last_values(input const &input);
        void reset();

        std::map<frame_index_t, input> const &inputs();
        T const *last_values() const;

       private:
        std::map<frame_index_t, input> _inputs;
        std::vector<T> _last_values;
    };
}
}

#include "yas_processing_number_process_context_private.h"
