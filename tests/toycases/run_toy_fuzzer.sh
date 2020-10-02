# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

rm *.o testexecutable *.h.gch
bash build_toy_base.sh 
bash build_toy_fuzz_parsers.sh

source ./paths_and_variables.sh

clang++ -g -fsanitize=address differential_fuzzer.o toy_fuzz_string_output_base.o toy_fuzz_int_parser.o \
toy_fuzz_switch_parser.o ./code/toytests.cpp \
-o toy_test_executable -I ../ -I ../../differential_fuzzing_lib/