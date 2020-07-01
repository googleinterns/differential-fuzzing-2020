#include "xmlparser.h"
#include "yamlparser.h"
#include "parser.h"
#include "differentialfuzzer.cpp"


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) 
{
    xaml_differential_parser::XmlParser xmlcase;
    yaml_differential_parser::YamlParser yamlcase;
    // parser* array_of_parsers[2] = {dynamic_cast<parser*>(&xmlcase), dynamic_cast<parser*>(&yamlcase)};

    // Parser* array_of_parsers[3] = {(Parser*)(&yamlcase), (Parser*)(&xmlcase),(Parser*)(&yamlcase)};
    // DifferentiallyFuzz(array_of_parsers, 3);

    // Parser* array_of_parsers[3] = {(Parser*)(&yamlcase),(Parser*)(&yamlcase), (Parser*)(&xmlcase)};
    // DifferentiallyFuzz(array_of_parsers, 3);

    differential_parser::Parser* array_of_parsers[3] = {(differential_parser::Parser*)(&yamlcase), 
        (differential_parser::Parser*)(&xmlcase),(differential_parser::Parser*)(&yamlcase),};
    differential_fuzzer::DifferentiallyFuzz(array_of_parsers, 3);

    return 0;
}