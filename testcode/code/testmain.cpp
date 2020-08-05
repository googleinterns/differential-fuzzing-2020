#include "testutils.cpp"

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    std::cout << "----------- libyaml tests -----------" << std::endl;

    // parseLibyaml(args[1]);

    std::string libyaml_error_string;

    std::string libyaml_final_output = parseLibyaml(args[1], &libyaml_error_string);

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

    std::string yamlcpp_final_output;

    std::string yamlcpp_error_msg;

    try
    {   
        std::ifstream stream_input_to_yamlcpp;

        stream_input_to_yamlcpp.open(args[1]);

        std::string input_to_yamlcpp;

        std::string temp_stream_parser_line;

        while(std::getline(stream_input_to_yamlcpp, temp_stream_parser_line))
        {
            input_to_yamlcpp += temp_stream_parser_line + "\n";
        }

        std::string::size_type prev = 0, current = 0;

        std::vector<YAML::Node> node = YAML::LoadAllFromFile(args[1]);

        for (std::vector<YAML::Node>::iterator it = node.begin(); 
            it != node.end(); it++)
        {
            std::cout << "Node:" << std::endl;
            std::string temp_result_holder = parseYamlCppNode(&(*it), &yamlcpp_error_msg);
            std::cout << temp_result_holder << std::endl;
            yamlcpp_final_output += temp_result_holder;

            if(temp_result_holder.empty() && yamlcpp_error_msg.empty())
            {
                yamlcpp_error_msg = "ERROR";
            }
        }

        if(!yamlcpp_error_msg.empty())
        {
            std::cout << "-------- Before error:" << std::endl;
            std::cout << yamlcpp_final_output << std::endl;
            yamlcpp_final_output = yamlcpp_error_msg;
            std::cout << "-------- " << std::endl;
        }
        std::cout << "(END)" << std::endl;
    }
    catch (const std::exception& err)
    {
        std::cout << "ERROR: yaml-cpp based parser error \n"<< yamlcpp_final_output << std::endl;
        yamlcpp_final_output = "ERROR";
    }

    std::cout << "--------yaml-cpp Output:" << std::endl;
    std::cout << yamlcpp_final_output << "(END)" << std::endl;
    std::cout << "--------" << std::endl;
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