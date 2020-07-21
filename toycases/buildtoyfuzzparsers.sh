clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c toyfuzz7parser.cpp toyfuzz9parser.cpp toyfuzzstringoutputbase.cpp differentialfuzzer.o \
-I ../ -I ./../../../libraries/yaml-cpp/include -I ./....//../libraries/libyaml/include
