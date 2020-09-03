#include "libyaml_parser.h"
#include "yamlcpp_parser.h"

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>

bool runTest(const char* file_path)
{
    FILE * file_descriptor = fopen(file_path, "rb");

    struct stat file_stat;

    if (fstat(fileno(file_descriptor), &file_stat) == 0)
    {   
        char buffer[file_stat.st_size + 1];

        if (fread(&buffer, sizeof(char), file_stat.st_size, file_descriptor))
        {
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

            bool return_me = (libyaml_test_normalized_output->equivalent(yamlcpp_test_normalized_output));

            delete yamlcpp_test_normalized_output;
            delete libyaml_test_normalized_output;

            return return_me;
        }
        else
        {
            std::cerr << "Failure reading to buffer" << std::endl;
        }
    }
    else
    {
        std::cerr << "Failure reading file" << std::endl;
    }
    return true;
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

    if ((dir = opendir (args[1])) != NULL) 
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) 
        {
            if (ent->d_name[0] != '.')
            {
                
                if (!runTest(std::string(std::string(args[1]) + std::string(ent->d_name)).c_str()))
                {
                    std::cout << std::string(std::string(args[1]) + std::string(ent->d_name)) << std::endl;
                    myfile << std::string(std::string(args[1]) + std::string(ent->d_name)) << std::endl;
                }
            }
        }
        closedir (dir);
    }
    return 0;
}
