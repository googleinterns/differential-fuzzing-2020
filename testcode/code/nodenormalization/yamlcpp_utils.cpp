#include "yamlcpp_utils.h"

std::vector<YAML::Node> parseYamlCpp(const std::string parse_me, 
                                            std::string* error_message_container)
{
    std::vector<YAML::Node> return_me;
    try
    {
        return_me = YAML::LoadAllFromFile(parse_me);
    }
    catch (const std::exception& err)
    {
        *error_message_container = "ERROR";
    }

    return return_me;
}

std::string normalizeYamlCpp(const std::vector<YAML::Node>* nodes, 
                                std::string* error_message_container)
{
    std::string yamlcpp_final_output;

    if (nodes->empty())
    {
        *error_message_container = "ERROR";
        return yamlcpp_final_output;
    }

    try
    {
        for (std::vector<YAML::Node>::const_iterator it = nodes->begin(); 
            it != nodes->end(); it++)
        {
            std::string temp_result_holder = normalizeYamlCppNode(&(*it), error_message_container);
            yamlcpp_final_output += temp_result_holder;

            if(temp_result_holder.empty() && error_message_container->empty())
            {
                *error_message_container = "ERROR";
            }
        }
    }
    catch (const std::exception& err)
    {
        *error_message_container = "ERROR";
    }

    return yamlcpp_final_output;
}


std::string normalizeYamlCppNode(const YAML::Node* head, std::string* error_message_container)
{
    std::stack <YAML::Node> iteration_list_stack;

    std::stack <std::string> additional_info_stack;

    iteration_list_stack.push(*head);
    additional_info_stack.push("U");

    std::string yamlcpp_final_output = "";

    while (!iteration_list_stack.empty())
    {
        YAML::Node base_iterator = iteration_list_stack.top();
        iteration_list_stack.pop();

        yamlcpp_final_output +=  additional_info_stack.top() + ": ";

        char current_mode =  additional_info_stack.top().front();

        additional_info_stack.pop();   

        const std::string& tag_holder = base_iterator.Tag();

        if (tag_holder != "?" && tag_holder != "!" && tag_holder != "")
        {
            yamlcpp_final_output += tag_holder + " ";
        }

        switch (base_iterator.Type())
        {    
            case YAML::NodeType::Null:
            {
                if (current_mode == 'U')
                {
                    yamlcpp_final_output = "";
                    return yamlcpp_final_output;
                }
                else
                {
                    *error_message_container = "ERROR";
                    return yamlcpp_final_output;
                }
            }
            case YAML::NodeType::Scalar:
            {
                yamlcpp_final_output +=  "- " + base_iterator.as<std::string>() + "\n";
                break;
            }
            case YAML::NodeType::Sequence:
            {
                yamlcpp_final_output += "\n";
                for (int i = base_iterator.size() - 1; i >= 0; i--) 
                {
                    iteration_list_stack.push(base_iterator[i]);
                    additional_info_stack.push("L");
                }

                break;
            }
            case YAML::NodeType::Map:
            {
                yamlcpp_final_output += "\n";
                std::stack <YAML::const_iterator> loca_iterators_temp_stack;

                for (YAML::const_iterator it = base_iterator.begin(); it != base_iterator.end(); ++it) 
                {
                    loca_iterators_temp_stack.push(it);
                }

                while (!loca_iterators_temp_stack.empty())
                {
                    YAML::const_iterator it = loca_iterators_temp_stack.top();
                    loca_iterators_temp_stack.pop();

                    iteration_list_stack.push(it->second);
                    additional_info_stack.push("V");
                    iteration_list_stack.push(it->first);
                    additional_info_stack.push("K");
                }
                break;
            }
            case YAML::NodeType::Undefined:
            {
                yamlcpp_final_output += "- Undef \n";

                break;
            }
            default:
            {
                yamlcpp_final_output += "- ERROR: Unknown Input Type \n";
            }
        }
    }

    if(yamlcpp_final_output.empty())
    {
        return "ERROR";
    }

    return yamlcpp_final_output;
}