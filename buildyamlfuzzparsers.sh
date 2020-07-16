clang++ ../../libraries/libyaml/src/.libs/libyaml.a ../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c libyamlparser.cpp yamlcppparser.cpp differentialfuzzer.o \
-I ./../../libraries/yaml-cpp/include -I ./../../libraries/libyaml/include \
-g -fsanitize=address,fuzzer
