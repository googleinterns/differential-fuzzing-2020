#include "baseparser.h"
#include "differentialfuzzer.h"
#include "yamlcppparser.h"
#include "libyamlparser.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t size) 
{
    libyaml_differential_parser::LibyamlParser libyaml_case;
    yamlcpp_differential_parser::YamlCppParser yaml_cpp_case;

    differential_parser::Parser* array_of_parsers[3] = {(differential_parser::Parser*)(&yaml_cpp_case), 
        (differential_parser::Parser*)(&libyaml_case),(differential_parser::Parser*)(&yaml_cpp_case),};
    
    differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 3, size);

    return 0;
}