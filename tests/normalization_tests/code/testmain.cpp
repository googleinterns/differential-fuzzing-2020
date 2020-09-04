#include "libyaml_parser.h"
#include "yamlcpp_parser.h"
#include "normalization_test_utils.h"

#include "logger.h"

#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    FILE * file_descriptor = fopen(args[1], "rb");

    struct stat file_stat;

    if (fstat(fileno(file_descriptor), &file_stat) == 0)
    {   
        char buffer[file_stat.st_size + 1];

        fread(&buffer, sizeof(char), file_stat.st_size, file_descriptor);
        std::cout << "----------- yaml-cpp tests -----------" << std::endl;
        std::vector<YAML::Node>* yamlcpp_test_normalized_output_data;

        yamlcpp_differential_parser::YamlCppParser* yamlcpp_case =
            yamlcpp_differential_parser::YamlCppParser::GetStaticInstance();

        std::unique_ptr<std::string> yamlcpp_error_string;
            yamlcpp_error_string = std::unique_ptr<std::string>(new std::string());

        void* yamlcpp_parsed_data = yamlcpp_case->parse((uint8_t*) buffer, file_stat.st_size, yamlcpp_error_string.get());

        differential_parser::NormalizedOutput* yamlcpp_test_normalized_output = yamlcpp_case->normalize
            (yamlcpp_parsed_data, std::move(yamlcpp_error_string));

        std::cout << "----------- libyaml tests -----------" << std::endl;

        std::vector<YAML::Node>* libyaml_test_normalized_output_data;

        libyaml_differential_parser::LibyamlParser* libyaml_case =
            libyaml_differential_parser::LibyamlParser::GetStaticInstance();

        std::unique_ptr<std::string> libyaml_error_string;
            libyaml_error_string = std::unique_ptr<std::string>(new std::string());

        void* libyaml_parsed_data = libyaml_case->parse((uint8_t*) buffer, file_stat.st_size, libyaml_error_string.get());

        differential_parser::NormalizedOutput* libyaml_test_normalized_output = libyaml_case->normalize
            (libyaml_parsed_data, std::move(libyaml_error_string));

        std::cout << "----------- compare -----------" << std::endl;

        // yamlcpp_test_normalized_output->getData()

        if (libyaml_test_normalized_output->equivalent(yamlcpp_test_normalized_output))
        {
            std::cout << "Cases equal!" << std::endl;
        }
        else
        {
            std::cout << "Cases different!" << std::endl;
        }

        delete yamlcpp_test_normalized_output;
        delete libyaml_test_normalized_output;
    }
    else
    {
        std::cerr << "Failure reading file" << std::endl;
    }
    
    return 0;
}