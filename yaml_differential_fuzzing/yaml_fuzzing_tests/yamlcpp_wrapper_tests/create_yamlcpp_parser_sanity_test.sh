bash cleanup.sh 

bash buildbase.sh 

bash buildyamlfuzzparsers.sh

source ./paths_and_variables.sh

clang++ differential_fuzzer.o comparison_utils.o  yamlcpp_parser.o ./code/test_yamlcpp_parser_fuzz.cpp \
yaml_normalized_output.o -I "$YAML_LIBRARY_PATHS"yaml-cpp/include -I "$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib/ \
"$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../../ -I ../../yamlparserlibraries -o yamlcpp_test_executable -g -fsanitize=address,fuzzer
