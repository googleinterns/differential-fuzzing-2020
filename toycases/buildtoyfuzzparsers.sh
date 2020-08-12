clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c ./code/toyfuzzintparser.cpp ./code/toyfuzzswitchparser.cpp ./code/toyfuzzstringoutputbase.cpp \
./differentialfuzzer.o \
-I ../ -I ./../../../libraries/yaml-cpp/include -I ./../../libraries/libyaml/include \
-I ../differentialfuzzinglib/ -I ./code
