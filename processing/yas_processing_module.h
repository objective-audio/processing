//
//  yas_processing_module.h
//

#pragma once

#include "yas_base.h"
#include <unordered_map>

namespace yas {
namespace processing {
    class stream;
    class time_range;
    class connector;

    class module : public base {
       protected:
        class impl;

        explicit module(std::shared_ptr<impl> &&);
        module(std::nullptr_t);

       public:
        using connector_map_t = std::unordered_map<std::string, connector>;

        void process(stream &);

        connector_map_t const &input_connectors();
        connector_map_t const &output_connectors();
        void connect_input(std::string const &key, int64_t const ch);
        void connect_output(std::string const &key, int64_t const ch);
        void disconnect_input(std::string const &key);
        void disconnect_output(std::string const &key);

        time_range const &time_range() const;
    };
}
}

#include "yas_processing_module_private.h"
