clang++ -g -fsanitize=address -DDEBUG_MODE \
-c ./code/toy_fuzz_int_parser.cpp ./code/toy_fuzz_switch_parser.cpp ./code/toy_fuzz_string_output_base.cpp \
./differential_fuzzer.o \
-I ../ -I ./../../../libraries/yaml-cpp/include -I ./../../libraries/libyaml/include \
-I ../differential_fuzzing_lib/ -I ./code
