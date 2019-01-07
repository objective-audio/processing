//
//  yas_processing_event.h
//

#pragma once

#include <cpp_utils/yas_base.h>

namespace yas::proc {
class time;

struct event : base {
    struct impl : base::impl {
        virtual bool validate_time(time const &) = 0;
        virtual event copy() = 0;
    };

   protected:
    explicit event(std::shared_ptr<impl> const &);
    explicit event(std::shared_ptr<impl> &&);

   public:
    event(std::nullptr_t);

    bool validate_time(time const &) const;

    event copy() const;
};
}  // namespace yas::proc
