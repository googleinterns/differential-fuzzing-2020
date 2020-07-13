#include "yamlcppparser.h"

#include "yaml-cpp/yaml.h"
#include <iostream>

namespace yamlcpp_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ YamlCppParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

YamlCppParserOutput::YamlCppParserOutput(std::string* info)
{
    this->data = info;
}

YamlCppParserOutput::~YamlCppParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool YamlCppParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* YamlCppParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlCppParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string YamlCppParser::getName()
{
    return "yaml-cpp";
}


void* YamlCppParser::parse(const uint8_t*input, size_t input_size)
{
     std::string* yaml_cpp_loop_temp = new std::string;


    if(input_size>0)
    {
        // YAML::Node primes = YAML::Load("[wookgie, woogie, it, is, the, spookie, boogie]");
        try
        {
            YAML::Node primes = YAML::Load("["+std::string((const char*)yaml_cpp_loop_temp, input_size)+"]");

            for (YAML::const_iterator it = primes.begin();it != primes.end(); ++it) 
            {
                *yaml_cpp_loop_temp += it->as<std::string>();
            }            
        }
        catch(const std::exception& e)
        {
            // std::cerr << e.what() << '\n';
        }
        

    }
    return (void*)yaml_cpp_loop_temp;
}

differential_parser::ParserOutput* YamlCppParser::normalize
    (void* input)
{   
    differential_parser::ParserOutput* returnMe = new
        YamlCppParserOutput((std::string*)input);
    
    return returnMe;
}
}