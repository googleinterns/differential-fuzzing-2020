source ./paths_and_variables.sh

clang++ "$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a \
-c ../../yamlparserlibraries/yamlcpp_parser.cpp ../../yamlparserlibraries/yaml_normalized_output.cpp \
../../yamlparserlibraries/utils/comparison_utils.cpp ./code/normalization_test_utils.cpp differential_fuzzer.o \
-I "$YAML_LIBRARY_PATHS"yaml-cpp/include -I "$YAML_LIBRARY_PATHS"libyaml/include \
-I ../../differential_fuzzing_lib -I ./../../yamlparserlibraries -I ../../ \
-g -fsanitize=address,fuzzer
