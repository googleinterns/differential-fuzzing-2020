
source ./paths_and_variables.sh

clang++ -g -fsanitize=address \
-c ./code/toy_fuzz_int_parser.cpp ./code/toy_fuzz_switch_parser.cpp ./code/toy_fuzz_string_output_base.cpp \
./differential_fuzzer.o \
-I "$DIFFERENTIAL_FUZZER_PATH" -I ./."$YAML_LIBRARY_PATHS"yaml-cpp/include -I "$YAML_LIBRARY_PATHS"libyaml/include \
-I "$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib/ -I ./code