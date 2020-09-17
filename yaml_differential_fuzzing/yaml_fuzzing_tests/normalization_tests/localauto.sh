bash buildbase.sh

bash buildyamlfuzzparsersauto.sh 

source ./paths_and_variables.sh

clang++ differential_fuzzer.o comparison_utils.o libyaml_utils.o normalization_test_utils.o yamlcpp_parser.o libyaml_parser.o  yaml_normalized_output.o \
./code/automated.cpp "$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a -I "$YAML_LIBRARY_PATHS"yaml-cpp/include \
-I ./"$YAML_LIBRARY_PATHS"libyaml/ -I ./code -I ../../ -I ../../yamlparserlibraries/ \
-I "$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib/ -g -fsanitize=address -o autotest