bash cleanup.sh 

bash buildbase.sh 

bash buildyamlfuzzparsers.sh

source ./paths_and_variables.sh

clang++ differential_fuzzer.o comparison_utils.o  yamlcpp_parser.o yaml_normalized_output.o ./code/test_yamlcpp_wrapper_check.cpp \
-I "$YAML_LIBRARY_PATHS"yaml-cpp/include -I ./../../differential_fuzzing_lib/ \
"$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../../ -I ../../yamlparserlibraries -o yaml_wrapper_check -g -fsanitize=address
