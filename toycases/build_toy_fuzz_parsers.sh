clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c ./code/toy_fuzz_int_parser.cpp ./code/toy_fuzz_switch_parser.cpp ./code/toy_fuzz_string_output_base.cpp \
./differential_fuzzer.o \
-I ../ -I ./../../../libraries/yaml-cpp/include -I ./../../libraries/libyaml/include \
-I ../differential_fuzzing_lib/ -I ./code
