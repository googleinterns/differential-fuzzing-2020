#include "base_parser.h"
#include "differential_fuzzer.h"
#include "yamlcpp_parser.h"
#include "libyaml_parser.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t size) 
{
    libyaml_differential_parser::LibyamlParser libyaml_case;
    yamlcpp_differential_parser::YamlCppParser yaml_cpp_case;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&yaml_cpp_case), 
        (differential_parser::Parser*)(&libyaml_case)};

    differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size);

    return 0;
}