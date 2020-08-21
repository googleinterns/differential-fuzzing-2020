#include <iostream>
#include <string>

#include "differential_fuzzer.h"
#include "yamlcpp_parser.h"

void simple_test(const uint8_t* Data, size_t size)
{
    yamlcpp_differential_parser::YamlCppParser yaml_cpp_case;

    std::cout << "----- Simple Test -----" << std::endl;
    std::cout << "----- Testing: " << yaml_cpp_case.getName() << std::endl;

    std::string *error_string = new std::string();

    differential_parser::NormalizedOutput* test_normalized_output = 
        yaml_cpp_case.normalize(yaml_cpp_case.parse(Data, size, error_string), error_string);

    std::cout << "---Error: "<< *test_normalized_output->getError() << std::endl;

    std::vector<YAML::Node>* test_normalized_output_data 
        = (std::vector<YAML::Node>*) test_normalized_output->getData();

    if (!test_normalized_output_data->empty())
    {
        std::cout << "--- Result: "  << std::endl << test_normalized_output_data->back() << std::endl;
    }
    delete test_normalized_output;
}

int main()
{
    simple_test((uint8_t*)"[1, 2]", 6);
    simple_test((uint8_t*)"Bob", 3);
    simple_test((uint8_t*)"[]", 2);
    simple_test((uint8_t*)"[&a a: b, *a]", 13);
    simple_test((uint8_t*)"[1, 2", 5);
    
    return 0;
}