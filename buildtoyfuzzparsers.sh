clang++ ../../libraries/libyaml/src/.libs/libyaml.a ../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c toyfuzz1parser.cpp toyfuzz2parser.cpp toyfuzz3parser.cpp toyfuzz4parser.cpp differentialfuzzer.o \
-I ./../../libraries/yaml-cpp/include -I ./../../libraries/libyaml/include
