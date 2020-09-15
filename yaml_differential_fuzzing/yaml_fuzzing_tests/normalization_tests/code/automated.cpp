#include "libyaml_parser.h"
#include "yamlcpp_parser.h"
#include "libyaml_parser.h"
#include "utils/comparison_utils.h"

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

enum class comparison_type_results
{
    SAME = 0, 
    DIFFERENT_ERROR = 1,
    DIFFERENT_DATA = 2,
    ERROR_IN_ONE = 3,
};

comparison_type_results CustomEquivalent(differential_parser::NormalizedOutput* compare_point_one, 
    differential_parser::NormalizedOutput* compare_point_two)
{
    std::vector<YAML::Node>* data_one = 
        static_cast<std::vector<YAML::Node>*>(compare_point_one->getData());

    std::vector<YAML::Node>* data_two = 
        static_cast<std::vector<YAML::Node>*>(compare_point_two->getData());

    if (compare_utils::CheckForEmpty(data_one) && compare_utils::CheckForEmpty(data_two))
    {
        return comparison_type_results::SAME;
    }

    if (!compare_point_one->getError()->empty() || !compare_point_two->getError()->empty())
    {
        if (*compare_point_one->getError() == *compare_point_two->getError())
        {
            return comparison_type_results::SAME;
        }
        else
        {
            return comparison_type_results::DIFFERENT_ERROR;
        }
    }
    else
    {
        if (data_one && data_two)
        {
            if (compare_utils::CompareMultipleNodes(data_one, data_two))
            {
                return comparison_type_results::SAME;
            }
            else
            {
                return comparison_type_results::DIFFERENT_DATA;
            }
        }
    }
    return comparison_type_results::DIFFERENT_DATA;
}

comparison_type_results runTest(const char* file_path)
{
    FILE * file_descriptor = fopen(file_path, "rb");

    struct stat file_stat;

    if (fstat(fileno(file_descriptor), &file_stat) == 0)
    {   
        char buffer[file_stat.st_size + 1];

        fread(&buffer, sizeof(char), file_stat.st_size, file_descriptor);

        std::vector<YAML::Node>* yamlcpp_test_normalized_output_data;

        yamlcpp_differential_parser::YamlCppParser* yamlcpp_case =
            yamlcpp_differential_parser::YamlCppParser::GetStaticInstance();

        std::unique_ptr<std::string> yamlcpp_error_string;
            yamlcpp_error_string = std::unique_ptr<std::string>(new std::string());

        void* yamlcpp_parsed_data = yamlcpp_case->parse((uint8_t*) buffer, file_stat.st_size, yamlcpp_error_string.get());

        differential_parser::NormalizedOutput* yamlcpp_test_normalized_output = yamlcpp_case->normalize
            (yamlcpp_parsed_data, std::move(yamlcpp_error_string));

        std::vector<YAML::Node>* libyaml_test_normalized_output_data;

        libyaml_differential_parser::LibyamlParser* libyaml_case =
            libyaml_differential_parser::LibyamlParser::GetStaticInstance();

        std::unique_ptr<std::string> libyaml_error_string;
            libyaml_error_string = std::unique_ptr<std::string>(new std::string());

        void* libyaml_parsed_data = libyaml_case->parse((uint8_t*) buffer, file_stat.st_size, libyaml_error_string.get());

        differential_parser::NormalizedOutput* libyaml_test_normalized_output = libyaml_case->normalize
            (libyaml_parsed_data, std::move(libyaml_error_string));

        comparison_type_results return_me = comparison_type_results::DIFFERENT_DATA;
        if (yamlcpp_test_normalized_output != nullptr)
        {
            return_me = CustomEquivalent(yamlcpp_test_normalized_output, libyaml_test_normalized_output);
            
            if (yamlcpp_test_normalized_output->equivalent(libyaml_test_normalized_output)
                && return_me != comparison_type_results::SAME)
            {
                std::cerr << "Comparison discrepancy with:\n" << file_path << std::endl;
            }
        }
        else
        {
            if (libyaml_test_normalized_output == nullptr)
            {
                return_me = comparison_type_results::SAME;
            }
        }

        if (libyaml_test_normalized_output != nullptr) delete libyaml_test_normalized_output;
        if (yamlcpp_test_normalized_output != nullptr) delete yamlcpp_test_normalized_output;

        return return_me;
    }
    else
    {
        std::cerr << "Failure reading file" << std::endl;
    }
    return comparison_type_results::SAME;
}

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------

int main(int argc, char* args[])
{
    DIR *dir;
    struct dirent *ent;

    std::ofstream myfile;

    myfile.open("autoreport.txt");

    std::vector<std::string> different_due_to_error;
    std::vector<std::string> different_due_to_data;

    if ((dir = opendir (args[1])) != NULL) 
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) 
        {
            if (ent->d_name[0] != '.')
            {
                comparison_type_results test_result = runTest(std::string(std::string(args[1]) + std::string(ent->d_name)).c_str());
                switch (test_result)
                {
                case (comparison_type_results::DIFFERENT_ERROR):
                    different_due_to_error.push_back(std::string(std::string(args[1]) + std::string(ent->d_name)));
                    break;

                case (comparison_type_results::DIFFERENT_DATA):
                    different_due_to_data.push_back(std::string(std::string(args[1]) + std::string(ent->d_name)));
                    break;
                default:
                    break;
                }
            }
        }
        closedir (dir);
    }

    std::cout << "---------------- Data Difference:" << std::endl;
    myfile << "---------------- Data Difference:" << std::endl;

    while (!different_due_to_data.empty())
    {
        std::cout << different_due_to_data.back() << std::endl;
        myfile << different_due_to_data.back() << std::endl;
        different_due_to_data.pop_back();
    }

    std::cout << "---------------- Error Difference:" << std::endl;
    myfile << "---------------- Error Difference:" << std::endl;

    while (!different_due_to_error.empty())
    {
        std::cout << different_due_to_error.back() << std::endl;
        myfile << different_due_to_error.back() << std::endl;
        different_due_to_error.pop_back();
    }
    return 0;
}