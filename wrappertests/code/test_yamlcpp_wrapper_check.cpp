#include <iostream>
#include <string.h>
#include <stdio.h>

#include "differential_fuzzer.h"
#include "yamlcpp_parser.h"

void SmokeTest(const uint8_t* Data)
{
    size_t size = strlen((const char*)Data);
    yamlcpp_differential_parser::YamlCppParser* temp_instance =
        yamlcpp_differential_parser::YamlCppParser::GetStaticInstance();

    std::cout << "----- Simple Test -----" << std::endl;
    std::cout << "----- Testing: " << temp_instance->getName() << std::endl;
    std::cout << "----- Memory of instance: " << temp_instance << std::endl;

    std::string *error_string = new std::string();

    differential_parser::NormalizedOutput* test_normalized_output = temp_instance->normalize
            (temp_instance->parse(Data, size, error_string), error_string);
    
    if (test_normalized_output != nullptr)
    {
        std::cerr << "---Error: "<< *test_normalized_output->getError() << std::endl;

        std::vector<YAML::Node>* test_normalized_output_data 
            = (std::vector<YAML::Node>*) test_normalized_output->getData();

        if (!test_normalized_output_data->empty())
        {
            std::cerr << "--- Result: "  << std::endl << test_normalized_output_data->back() << std::endl;
        }
    }
    else
    {
        delete error_string;
    }
    delete test_normalized_output;
}

int main()
{
    SmokeTest((uint8_t*)"[1, 2]");
    SmokeTest((uint8_t*)"Bob");
    SmokeTest((uint8_t*)"[]");
    SmokeTest((uint8_t*)"[&a a: b, *a]");
    SmokeTest((uint8_t*)"[1, 2");
    
    return 0;
}