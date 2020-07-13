rm *.o testexecutable *.h.gch
bash buildbase.sh 
bash buildyamlfuzzparsers.sh
clang++ differentialfuzzer.o  libyamlparser.o  yamlcppparser.o testfuzz.cpp -I ./../../libraries/yaml-cpp/include ../../libraries/libyaml/src/.libs/libyaml.a ../../libraries/yaml-cpp/build/libyaml-cpp.a -o testexecutable -g -fsanitize=address,fuzzer
