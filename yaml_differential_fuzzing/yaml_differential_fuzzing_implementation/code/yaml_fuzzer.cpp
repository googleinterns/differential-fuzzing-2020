// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "base_parser.h"
#include "differential_fuzzer.h"
#include "libyaml_parser.h"
#include "yamlcpp_parser.h"

#include <time.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t size) 
{
    differential_parser::Parser* array_of_parsers[2] = {
        (differential_parser::Parser*)(libyaml_differential_parser::LibyamlParser::GetStaticInstance()), 
        (differential_parser::Parser*)(yamlcpp_differential_parser::YamlCppParser::GetStaticInstance())};

    bool fuzzers_are_equal = 
        differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size);

    assert(fuzzers_are_equal);

    return 0;
}
