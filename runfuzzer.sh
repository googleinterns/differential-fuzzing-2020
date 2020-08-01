rm *.o testexecutable *.h.gch
bash buildbase.sh 
bash buildyamlfuzzparsers.sh
clang++ differentialfuzzer.o  libyamlparser.o  yamlcppparser.o ./testcode/testfuzz.cpp \
-I ./../../libraries/yaml-cpp/include -I ./differentialfuzzinglib/ \
../../libraries/libyaml/src/.libs/libyaml.a ../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./testcode -I ./ -I ./yamlparserlibraries -o testexecutable -g -fsanitize=address,fuzzer
