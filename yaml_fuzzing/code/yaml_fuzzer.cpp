#include "base_parser.h"
#include "differential_fuzzer.h"
#include "libyaml_parser.h"
#include "yamlcpp_parser.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t size) 
{
    differential_parser::Parser* array_of_parsers[2] = {
        (differential_parser::Parser*)(libyaml_differential_parser::LibyamlParser::GetStaticInstance()), 
        (differential_parser::Parser*)(yamlcpp_differential_parser::YamlCppParser ::GetStaticInstance())};

    bool fuzzers_are_different = 
        differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size);

    assert(fuzzers_are_different);

    return 0;
}