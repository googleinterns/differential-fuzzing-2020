rm *.o testexecutable
bash build_toy_base.sh 
bash build_toy_fuzz_parsers.sh

clang++ -g -fsanitize=address differential_fuzzer.o toy_fuzz_string_output_base.o toy_fuzz_int_parser.o \
toy_fuzz_switch_parser.o ./code/toytests.cpp \
-o toy_test_executable -I ../ -I ../differential_fuzzing_lib/