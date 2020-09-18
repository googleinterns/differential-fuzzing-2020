
source ./paths_and_variables.sh

clang++ -g -fsanitize=address -DDEBUG_MODE \
-c "$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib/differential_fuzzer.cpp -I \
"$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib/