source ./paths_and_variables.sh

clang++ -g -fsanitize=address,fuzzer -c "$DIFFERENTIAL_FUZZER_PATHS"differential_fuzzing_lib/differential_fuzzer.cpp 