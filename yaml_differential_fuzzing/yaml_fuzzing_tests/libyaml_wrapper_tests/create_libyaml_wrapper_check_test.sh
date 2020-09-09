bash cleanup.sh 

bash buildbase.sh 

bash buildyamlfuzzparsers.sh

source ./paths_and_variables.sh

clang++ differential_fuzzer.o comparison_utils.o libyaml_utils.o libyaml_parser.o yamlcpp_parser.o ./code/test_libyaml_wrapper_check.cpp \
yaml_normalized_output.o -I "$YAML_LIBRARY_PATHS"yaml-cpp/include -I "$YAML_LIBRARY_PATHS"libyaml/ \
-I ./../../../differential_fuzzing_lib/ \
"$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../../ -I ../../yamlparserlibraries -o libyaml_wrapper_check -g -fsanitize=address
