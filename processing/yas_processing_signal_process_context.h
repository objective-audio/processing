//
//  yas_processing_signal_process_context.h
//

#pragma once

#include "yas_processing_signal_event.h"
#include "yas_processing_time.h"

namespace yas {
namespace processing {
    template <typename T, std::size_t N>
    struct signal_process_context {
        signal_process_context();

        void reset(std::size_t const reserve_size);

        std::vector<signal_event> const &signals() const;
        std::vector<time> const &times() const;

        void copy_data_from(T const *ptr, std::size_t const size, std::size_t const idx);
        T const *data(std::size_t const idx) const;

        void set_time(processing::time time, std::size_t const idx);
        time const &time(std::size_t const idx) const;

       private:
        std::vector<signal_event> _signals;
        std::vector<processing::time> _times;
    };
}
}

#include "yas_processing_signal_process_context_private.h"
