#include "utils.h"

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    std::cout << "----------- libyaml tests -----------" << std::endl;

    // normalizeLibyaml(args[1]);

    std::string libyaml_error_string;

    std::string libyaml_final_output = normalizeLibyaml(parseLibyaml(args[1], &libyaml_error_string), &libyaml_error_string);

    std::cout << "----------- libyaml output:" << std::endl;

    if (!libyaml_error_string.empty())
    {
        std::cout << "-------- Before error:" << std::endl;
        std::cout << libyaml_final_output << std::endl;
        libyaml_final_output = libyaml_error_string;
        std::cout << "-------- " << std::endl;
    }

    std::cout << libyaml_final_output << "(END)" << std::endl;

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;
    
    std::string yamlcpp_error_msg;

    std::vector<YAML::Node> parsed_nodes= parseYamlCpp(args[1], &yamlcpp_error_msg);

    std::string yamlcpp_final_output = normalizeYamlCpp
                (&parsed_nodes, &yamlcpp_error_msg);

    std::cout << "--------yaml-cpp Output:" << std::endl;
    std::cout << yamlcpp_final_output << "(END)" << std::endl;
    std::cout << "--------" << std::endl;

    if (!yamlcpp_error_msg.empty())
    {
        yamlcpp_final_output = yamlcpp_error_msg;
    }
    std::cout << "- Conclusion: " << std::endl;

    std::string buffer;

    if (compareStringsCustom(libyaml_final_output, yamlcpp_final_output, buffer))
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

// \0 in the middle