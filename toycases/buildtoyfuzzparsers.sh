clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c toyfuzz1parser.cpp toyfuzz2parser.cpp toyfuzz3parser.cpp toyfuzz4parser.cpp toyfuzz5parser.cpp \
toyfuzz6parser.cpp toyfuzz7parser.cpp toyfuzz8parser.cpp differentialfuzzer.o \
-I ../ -I ./../../../libraries/yaml-cpp/include -I ./....//../libraries/libyaml/include
