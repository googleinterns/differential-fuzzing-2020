clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c ../yamlparserlibraries/libyamlparser.cpp ../yamlparserlibraries/yamlcppparser.cpp \
differential_fuzzer.o \
-I ./../../../libraries/yaml-cpp/include -I ./../../../libraries/libyaml/include \
-I ../differential_fuzzing_lib -I ./../yamlparserlibraries -I ../ \
-g -fsanitize=address,fuzzer
