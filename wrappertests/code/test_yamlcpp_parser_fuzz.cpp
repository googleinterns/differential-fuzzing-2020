#include "base_parser.h"
#include "differential_fuzzer.h"
#include "yamlcpp_parser.h"

#include <assert.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t size) 
{
    yamlcpp_differential_parser::YamlCppParser yaml_cpp_case;
    //Temporary placeholder for checking integration with yaml-cpp
    yamlcpp_differential_parser::YamlCppParser yaml_cpp_case_two;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&yaml_cpp_case), 
        (differential_parser::Parser*)(&yaml_cpp_case_two)};

    assert(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));

    return 0;
}