#!/bin/sh

clang-format -i -style=file `find ../processing ../processing_mac_sample ../processing_tests -type f \( -name *.h -o -name *.cpp -o -name *.hpp -o -name *.m -o -name *.mm \)`
