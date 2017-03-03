//
//  yas_processing_cast_module.cpp
//

#include "yas_processing_cast_module.h"

using namespace yas;

void yas::connect(processing::module &module, processing::cast::input const &input,
                  processing::channel_index_t const &ch_idx) {
    module.connect_input(processing::to_connector_index(input), ch_idx);
}

void yas::connect(processing::module &module, processing::cast::output const &output,
                  processing::channel_index_t const &ch_idx) {
    module.connect_output(processing::to_connector_index(output), ch_idx);
}

std::string yas::to_string(processing::cast::input const &input) {
    using namespace yas::processing::cast;

    switch (input) {
        case input::value:
            return "value";
    }

    throw "input not found.";
}

std::string yas::to_string(processing::cast::output const &output) {
    using namespace yas::processing::cast;

    switch (output) {
        case output::value:
            return "value";
    }

    throw "output not found.";
}
