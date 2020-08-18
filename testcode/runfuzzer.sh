rm *.o testexecutable *.h.gch

bash buildbase.sh 

bash buildyamlfuzzparsers.sh

clang++ differentialfuzzer.o  libyamlparser.o  yamlcppparser.o ./code/testfuzz.cpp \
-I ./../../../libraries/yaml-cpp/include -I ./../differentialfuzzinglib/ \
../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../ -I ../yamlparserlibraries -o testexecutable -g -fsanitize=address,fuzzer
