#include <iostream>
#include <string.h>
#include <stdio.h>

#include "yamlcpp_parser.h"
#include "libyaml_parser.h"

void SmokeTest(const uint8_t* data)
{
    size_t size = strlen((const char*)data);
    libyaml_differential_parser::LibyamlParser* temp_instance = 
        libyaml_differential_parser::LibyamlParser::GetStaticInstance();

    std::cout << "----- Simple Test -----" << std::endl;
    std::cout << "----- Testing: " << temp_instance->getName() << std::endl;
    std::cout << "----- Memory of instance: " << temp_instance << std::endl;

    std::unique_ptr<std::string> error_string;
    error_string = std::unique_ptr<std::string>(new std::string());

    void* parsed_data = temp_instance->parse(data, size, error_string.get());
    
    differential_parser::NormalizedOutput* test_normalized_output = 
        temp_instance->normalize(parsed_data, std::move(error_string));
    
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
    delete test_normalized_output;
}

differential_parser::NormalizedOutput* ParseInfo(const uint8_t* data, std::string error, 
    libyaml_differential_parser::LibyamlParser* libyaml_case)
{
    size_t size = strlen((const char*)data);

    std::unique_ptr<std::string> error_string;
    error_string = std::unique_ptr<std::string>(new std::string());

    void* parsed_data = libyaml_case->parse(data, size, error_string.get());
    
    differential_parser::NormalizedOutput* test_normalized_output = libyaml_case->normalize
            (parsed_data, std::move(error_string));

    if (test_normalized_output != nullptr)
    {
        std::vector<YAML::Node>* test_normalized_output_data 
            = (std::vector<YAML::Node>*) test_normalized_output->getData();
    }
    *test_normalized_output->getError() = std::string(error);

    return test_normalized_output;
}

void InternalComparisonMechanismTest(const uint8_t* data_one, const uint8_t* data_two,
    std::string error_one, std::string error_two)
{
    libyaml_differential_parser::LibyamlParser libyaml_case_one;
    libyaml_differential_parser::LibyamlParser libyaml_case_two;

    differential_parser::NormalizedOutput* point_one = 
        ParseInfo(data_one, error_one, &libyaml_case_one);

    differential_parser::NormalizedOutput* point_two = 
        ParseInfo(data_two, error_two, &libyaml_case_two);

    std::cout << "---- Mechanism Test ----" << std::endl;
    std::cout << "----- Testing: " << libyaml_case_one.getName() << std::endl;

    if (point_one->equivalent(point_two) == point_two->equivalent(point_one))
    {
        std::cout << "- Cases equal" << std::endl;
        if (point_one->equivalent(point_two) && point_two->equivalent(point_one))
        {
            std::cout << "- Equivalent" << std::endl;
        }
        else
        {
            std::cout << "- Different" << std::endl;
        }
    }
    else
    {
        std::cout << "- Cases unequal" << std::endl;
    }

    delete point_one;
    delete point_two;
}

int main()
{
    SmokeTest((uint8_t*) "[1, 2]");
    SmokeTest((uint8_t*) "Bob");
    SmokeTest((uint8_t*) "[]");
    SmokeTest((uint8_t*) "[&a a: b, *a]");
    SmokeTest((uint8_t*) "[1, 2");
    SmokeTest((uint8_t*) "[*abc, &abc 2]");

    // Same data, and same error:
    std::cout << "--------- Same data, and same error ---------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Equivalent" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2]", (uint8_t*) "[1, 2]","", "");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "Bob","", "");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "[]","", "");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a a: b, *a]","", "");
    std::cout << "--------- Error + Empty Test ---------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Equivalent" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "[1, 2","", "");

    // Same data, but 1 error
    std::cout << "----------- Same data, but 1 error ----------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Different" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2]", (uint8_t*) "[1, 2]","", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "Bob","Oops", "");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "[]","", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a a: b, *a]","Oops", "");
    std::cout << "--------- Error + Empty Test ---------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Equivalent" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "[1, 2","", "Oops");

    // Same data, but contain different error
    std::cout << "------- Same data, but different error ------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Different" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2]", (uint8_t*) "[1, 2]", "Woops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "Bob","Oops",  "Woops");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "[]", "Woops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a a: b, *a]","Oops",  "Woops");
        std::cout << "--------- Error + Empty Test ---------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Equivalent" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "[1, 2", "Woops", "Oops");

    // Same data, but contain same error
    std::cout << "--------- Same data, but same error --------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Equivalent" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2]", (uint8_t*) "[1, 2]", "Oops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "Bob","Oops",  "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "[]", "Oops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a a: b, *a]","Oops",  "Oops");
    std::cout << "--------- Error + Empty Test ---------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Equivalent" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "[1, 2", "Oops", "Oops");

    // Different data, and same error:
    std::cout << "------- Different data, and same error -------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Different" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "BoB", (uint8_t*) "[1, 2]","", "");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "{Bob: test}","", "");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "{}","", "");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a e: b, *a]","", "");
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "{fork: now}","", "");

    // Different data, but 1 error
    std::cout << "--------- Different data, but 1 error --------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Different" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "BoB", (uint8_t*) "[1, 2]","", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "{Bob: test}","Oops", "");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "{}","", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a e: b, *a]","Oops", "");
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "{fork: now}","", "Oops");

    // Different data, but contain different error
    std::cout << "----- Different data, but different error ----" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Different" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "BoB", (uint8_t*) "[1, 2]", "Woops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "{Bob: test}","Oops",  "Woops");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "{}", "Woops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a e: b, *a]","Oops",  "Woops");
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "{fork: now}", "Woops", "Oops");

    // Different data, but contain same error
    std::cout << "------- Different data, but same error ------" << std::endl;
    std::cout << "----Expected output:" << std::endl;
    std::cout << "- Cases equal" << std::endl;
    std::cout << "- Equivalent" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "BoB", (uint8_t*) "[1, 2]", "Oops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "Bob", (uint8_t*) "{Bob: test}","Oops",  "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[]", (uint8_t*) "{}", "Oops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[&a a: b, *a]", (uint8_t*) "[&a e: b, *a]","Oops",  "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "{fork: now}", "Oops", "Oops");

    std::cout << "---------------- Just errors ---------------" << std::endl;
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "[fork, spoon", "Oops", "Oops");
    InternalComparisonMechanismTest((uint8_t*) "[1, 2", (uint8_t*) "[fork, spoon", "waps", "Oops");
    
    return 0;
}