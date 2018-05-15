//
//  yas_processing_cast_module.cpp
//

#include "yas_processing_cast_module.h"

using namespace yas;

void yas::connect(proc::module &module, proc::cast::input const &input,
                  proc::channel_index_t const &ch_idx) {
  module.connect_input(proc::to_connector_index(input), ch_idx);
}

void yas::connect(proc::module &module, proc::cast::output const &output,
                  proc::channel_index_t const &ch_idx) {
  module.connect_output(proc::to_connector_index(output), ch_idx);
}

std::string yas::to_string(proc::cast::input const &input) {
  using namespace yas::proc::cast;

  switch (input) {
  case input::value:
    return "value";
  }

  throw "input not found.";
}

std::string yas::to_string(proc::cast::output const &output) {
  using namespace yas::proc::cast;

  switch (output) {
  case output::value:
    return "value";
  }

  throw "output not found.";
}
