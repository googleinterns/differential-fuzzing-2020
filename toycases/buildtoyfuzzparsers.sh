clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c toyfuzzintparser.cpp toyfuzzswitchparser.cpp toyfuzzstringoutputbase.cpp differentialfuzzer.o \
-I ../ -I ./../../../libraries/yaml-cpp/include -I ./../../libraries/libyaml/include \
-I ../differentialfuzzinglib/
