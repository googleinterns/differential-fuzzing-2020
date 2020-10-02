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

bash buildbase.sh

bash buildyamlfuzzparsers.sh 

source ./paths_and_variables.sh

clang++ differential_fuzzer.o comparison_utils.o libyaml_utils.o normalization_test_utils.o yamlcpp_parser.o libyaml_parser.o  yaml_normalized_output.o \
./code/testmain.cpp "$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a -I "$YAML_LIBRARY_PATHS"yaml-cpp/include \
-I ./"$YAML_LIBRARY_PATHS"libyaml/ -I ./code -I ../../ -I ../../yamlparserlibraries/ \
-I "$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib/ -g -fsanitize=address -o manualtest