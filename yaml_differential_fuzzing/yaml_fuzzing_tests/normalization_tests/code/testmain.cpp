// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
    std::vector<YAML::Node>* casted_input = static_cast<std::vector<YAML::Node>*>(nullptr);
    if (casted_input == nullptr)
    {
        std::cout << "Nice" << std::endl;
    }
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

        differential_parser::NormalizedOutput* yamlcpp_test_normalized_output = 
            yamlcpp_case->normalize(yamlcpp_parsed_data, std::move(yamlcpp_error_string));

        if (!yamlcpp_test_normalized_output->getError()->empty())
        {
            std::cout << "----- Error present" << std::endl;
        }

        std::vector<YAML::Node>* yamlcpp_data = 
            static_cast<std::vector<YAML::Node>*>((yamlcpp_test_normalized_output)->getData());

        if (!yamlcpp_data->empty())
        {
            std::cout << "----- Size:"<< yamlcpp_data->size() << std::endl;
            PrintNodes(yamlcpp_data);
        }

        std::cout << "----------- libyaml tests -----------" << std::endl;

        std::vector<YAML::Node>* libyaml_test_normalized_output_data;

        libyaml_differential_parser::LibyamlParser* libyaml_case =
            libyaml_differential_parser::LibyamlParser::GetStaticInstance();

        std::unique_ptr<std::string> libyaml_error_string;
            libyaml_error_string = std::unique_ptr<std::string>(new std::string());

        void* libyaml_parsed_data = libyaml_case->parse((uint8_t*) buffer, file_stat.st_size, libyaml_error_string.get());

        differential_parser::NormalizedOutput* libyaml_test_normalized_output = 
            libyaml_case->normalize(libyaml_parsed_data, std::move(libyaml_error_string));

        if (!libyaml_test_normalized_output->getError()->empty())
        {
            std::cout << "----- Error present" << std::endl;
        }

        std::vector<YAML::Node>* libyaml_data = 
            static_cast<std::vector<YAML::Node>*>((libyaml_test_normalized_output)->getData());

        if (!libyaml_data->empty())
        {
            std::cout << "----- Size:"<< libyaml_data->size() << std::endl;
            PrintNodes(libyaml_data);
        }
        std::cout << "----------- compare manual -----------" << std::endl;

        if (yamlcpp_test_normalized_output->equivalent(libyaml_test_normalized_output))
        {
            std::cout << "Cases equal!" << std::endl;
            differential_parser::Parser* array_of_parsers[2] = { (differential_parser::Parser*)(libyaml_differential_parser::LibyamlParser::GetStaticInstance()),
                (differential_parser::Parser*)(yamlcpp_differential_parser::YamlCppParser::GetStaticInstance())};

            bool fuzzers_are_equal = 
                differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, (uint8_t*) buffer, file_stat.st_size);

            assert(fuzzers_are_equal);

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