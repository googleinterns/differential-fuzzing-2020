#include "libyaml_utils.h"
#include "yamlcpp_utils.h"
#include "comparison_utils.h"
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

    std::string old_error;

    std::string libyaml_final_string_output = normalizeYamlCpp(&libyaml_final_output, &old_error);

    std::cout << "----------- libyaml tests -----------" << std::endl;

    if (libyaml_error_string.empty())
    {
        std::cout << libyaml_final_string_output << "(END)" << std::endl;
    }
    else
    {
        std::cout << libyaml_error_string << std::endl;
    }

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;

    std::string yamlcpp_error_msg;

    std::vector<YAML::Node> parsed_nodes = parseYamlCpp(args[1], &yamlcpp_error_msg);

    std::string yamlcpp_final_output = normalizeYamlCpp
                (&parsed_nodes, &old_error);

    std::cout << "--------yaml-cpp Output:" << std::endl;

    if (yamlcpp_error_msg.empty())
    {
        std::cout << yamlcpp_final_output << "(END)" << std::endl;
    }
    else
    {
        std::cout << yamlcpp_final_output << "(END)" << std::endl;
        std::cout << yamlcpp_error_msg << std::endl;
    }

    std::cout << "--------" << std::endl;

    std::cout << "- Conclusion: " << std::endl;

    std::string buffer;

    // if (libyaml_error_string == yamlcpp_error_msg)
    // {
    //     if (libyaml_error_string.empty())
    //     {
    //         if (compareMultipleNodes(&libyaml_final_output, &parsed_nodes))
    //         {
    //             std::cout << "Cases equal!" << std::endl;
    //         }
    //         else
    //         {
    //             std::cout << buffer << std::endl;  
    //             std::cout << "Cases different!" << std::endl;
    //         }
    //     }
    //     else
    //     {
    //         std::cout << "Cases equal!" << std::endl;
    //     }
    // }
    // else
    // {
    //     std::cout << "Cases different!" << std::endl;
    // }

    if( (!libyaml_error_string.empty() || !yamlcpp_error_msg.empty()))
    {
        if (libyaml_error_string == yamlcpp_error_msg)
        {
            std::cout << "Cases equal!" << std::endl;
        }
        else
        {
            std::cout << "Cases different!" << std::endl;
        }
    }
    else
    {
        if (compareMultipleNodes(&libyaml_final_output, &parsed_nodes))
        {
            std::cout << "Cases equal!" << std::endl;
        }
        else
        {
            std::cout << "Cases different!" << std::endl;
        }
    }
    
    return 0;
}