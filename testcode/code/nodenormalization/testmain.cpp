#include "utils.h"

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    // YAML::Node temp_test1 = YAML::Load("%TAG !e! tag:example.com,2000:app/\n---\n[hello: [3, 34, !e!tag%21 makr]]");
    // YAML::Node temp_test2 = YAML::Load("%TAG !e! tag:example.com,2000:app/\n---\n[hello: [3, 34, !e!tag%22 makr]]");
    // std::string buffer;

    // if (compareSingleNode(&temp_test1, &temp_test2))
    // {
    //     std::cout << "TRUE" << std::endl;
    // }
    // else
    // {
    //     std::cout << "FALSE" << std::endl;
    // }


    std::string libyaml_error_string = "";

    std::vector<YAML::Node> libyaml_final_output = 
        normalizeLibyaml(parseLibyaml(args[1], &libyaml_error_string), &libyaml_error_string);

    std::string libyaml_final_string_output = normalizeYamlCpp(&libyaml_final_output, &libyaml_error_string);

    std::cout << "----------- libyaml tests -----------" << std::endl;

    if (!libyaml_error_string.empty())
    {
        std::cout << "-------- Before error:" << std::endl;
        std::cout << libyaml_final_string_output << std::endl;
        libyaml_final_string_output = libyaml_error_string;
        std::cout << "-------- " << std::endl;
    }
    else
    {   
        std::cout << libyaml_final_string_output << std::endl;
    }

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;

    std::string yamlcpp_error_msg;

    std::vector<YAML::Node> parsed_nodes = parseYamlCpp(args[1], &yamlcpp_error_msg);

    std::string yamlcpp_final_output = normalizeYamlCpp
                (&parsed_nodes, &yamlcpp_error_msg);

    std::cout << "--------yaml-cpp Output:" << std::endl;
    std::cout << yamlcpp_final_output << "(END)" << std::endl;
    std::cout << "--------" << std::endl;

    if (!yamlcpp_error_msg.empty())
    {
        yamlcpp_final_output = yamlcpp_error_msg;
        std::cout << "ERROR" << std::endl;
    }
    std::cout << "- Conclusion: " << std::endl;

    std::string buffer;

    if (compareMultipleNodes(&libyaml_final_output, &parsed_nodes))
    {
        std::cout << buffer << std::endl;  
        std::cout << "Cases equal!" << std::endl;
    }
    else
    {
        std::cout << buffer << std::endl;  
        std::cout << "(END)" << std::endl;
        std::cout << "Cases different!" << std::endl;
    }

    return 0;
}