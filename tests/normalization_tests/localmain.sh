bash buildbase.sh

bash buildyamlfuzzparsers.sh 

source ./paths_and_variables.sh

clang++ ./code/testmain.cpp comparison_utils.o  differential_fuzzer.o  normalization_test_utils.o  yamlcpp_parser.o  yaml_normalized_output.o \
"$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a -I "$YAML_LIBRARY_PATHS"yaml-cpp/include \
-I ./"$YAML_LIBRARY_PATHS"libyaml/include -I ./code -I ../../ -I ../../yamlparserlibraries/ \
-I ../../differential_fuzzing_lib/ -g -fsanitize=address -o manualtest