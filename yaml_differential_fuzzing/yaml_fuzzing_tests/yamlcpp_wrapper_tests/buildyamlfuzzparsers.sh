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

source ./paths_and_variables.sh

clang++ "$YAML_LIBRARY_PATHS"libyaml/src/.libs/libyaml.a "$YAML_LIBRARY_PATHS"yaml-cpp/build/libyaml-cpp.a \
-c ../../yamlparserlibraries/yamlcpp_parser.cpp ../../yamlparserlibraries/yaml_normalized_output.cpp \
../../yamlparserlibraries/utils/comparison_utils.cpp differential_fuzzer.o \
-I "$YAML_LIBRARY_PATHS"yaml-cpp/include -I "$YAML_LIBRARY_PATHS"libyaml/ \
-I "$DIFFERENTIAL_FUZZER_PATH"differential_fuzzing_lib -I ./../../yamlparserlibraries -I ../../ \
-g -fsanitize=address,fuzzer
