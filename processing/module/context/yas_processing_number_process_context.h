//
//  yas_processing_number_process_context.h
//

#pragma once

#include <processing/yas_processing_time.h>
#include <processing/yas_processing_types.h>

#include <map>
#include <optional>
#include <vector>

namespace yas::proc {
template <typename T, std::size_t N>
struct number_process_context {
    struct input {
        std::optional<T> values[N];
    };

    number_process_context();

    void insert_input(frame_index_t const &frame, T const &value, std::size_t const idx);
    void update_last_values(input const &input);
    void reset(time::range const &);

    std::map<frame_index_t, input> const &inputs() const;
    std::vector<T> const &last_values() const;

   private:
    std::map<frame_index_t, input> _inputs;
    std::vector<T> _last_values;
    std::optional<time::range> _last_process_range;
};
}  // namespace yas::proc

#include <processing/yas_processing_number_process_context_private.h>
