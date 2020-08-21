rm *.o testexecutable *.h.gch

bash buildbase.sh 

bash buildyamlfuzzparsers.sh

clang++ differential_fuzzer.o comparison_utils.o  yamlcpp_parser.o ./code/test_yamlcpp_wrapper_check.cpp \
-I ./../../../libraries/yaml-cpp/include -I ./../differential_fuzzing_lib/ \
../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../ -I ../yamlparserlibraries -o yamlwrappercheck -g -fsanitize=address
