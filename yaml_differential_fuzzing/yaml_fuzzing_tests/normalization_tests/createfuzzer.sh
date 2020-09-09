rm *.o testexecutable *.h.gch

bash buildbase.sh 

bash buildyamlfuzzparsers.sh

clang++ differential_fuzzer.o comparison_utils.o libyaml_parser.o  yamlcpp_parser.o ./code/testfuzz.cpp \
-I ./../../../libraries/yaml-cpp/include -I ./../differential_fuzzing_lib/ \
../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../ -I ../yamlparserlibraries -o testexecutable -g -fsanitize=address,fuzzer