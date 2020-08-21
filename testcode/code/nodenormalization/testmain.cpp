#include "libyaml_utils.h"
#include "yamlcpp_utils.h"
#include "comparison_utils.h"
#include "utils.h"

#include "logger.h"

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    std::string libyaml_error_string = "";

    TEST_PPRINT("Check1\n");
    std::vector<YAML::Node> libyaml_final_output = 
        normalizeLibyaml(parseLibyaml(args[1], &libyaml_error_string), &libyaml_error_string);

    TEST_PPRINT("Check2\n");
    std::string old_error;

    std::string libyaml_final_string_output = normalizeYamlCpp(&libyaml_final_output, &old_error);

    TEST_PPRINT("Check3\n");
    std::cout << "----------- libyaml tests -----------" << std::endl;

    if (!libyaml_final_output.empty())
    {
        std::cout << libyaml_final_output.back() << std::endl;
    }

    std::cout << "-------- libyaml Output:" << std::endl;

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

    if (!parsed_nodes.empty())
    {
        std::cout << parsed_nodes.back() << std::endl;
    }

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