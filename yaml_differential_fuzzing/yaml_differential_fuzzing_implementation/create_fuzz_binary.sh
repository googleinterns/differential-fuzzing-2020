bash buildbase.sh

bash buildyamlfuzzparsers.sh 

source ./paths_and_variables.sh

clang++ differential_fuzzer.o comparison_utils.o libyaml_utils.o yamlcpp_parser.o libyaml_parser.o  yaml_normalized_output.o \
./code/yaml_fuzzer.cpp "$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a -I "$YAML_LIBRARY_PATHS"yaml-cpp/include \
-I ./"$YAML_LIBRARY_PATHS"libyaml/ -I ./code -I ../../ -I ../yamlparserlibraries/ \
-I "$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib/ -I "$DIFFERENTIAL_FUZZER_PATHS"differential_fuzzing_lib/ \
-g -fsanitize=address,fuzzer -o yaml_fuzzer