#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <assert.h>
#include <fstream>

// From git file
#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

bool positionAnalysis(std::string& add_to_me, char reference_character, bool map_mode)
{
    if (reference_character == 'M')
    {
        if (map_mode)
        {
            add_to_me += "K: ";
        }
        else
        {
            add_to_me += "V: ";
        }
        return !map_mode;
    }
    else if (reference_character == 'S')
    {
        add_to_me += "L: ";
    }
    else
    {
        add_to_me += "U: ";
    }

    return map_mode;
}

void addToMap(std::map<std::string, std::string>& anchor_map, 
    std::string& anchor, std::string& anchor_data)
{
    if (&anchor != nullptr)
    {
        if (anchor_map.count(anchor))
        {
            anchor_map[anchor] = anchor_data;
        }
        else
        {
            anchor_map.insert({anchor, anchor_data});
        }
    }
}

bool addToMapDirective(std::map<std::string, std::string>& anchor_map, 
    std::stack<std::string>& anchor_save_stack, std::stack<std::string>& anchor_data_save_stack,
    int& subtract_count, bool interest_in_saving)
{
    if (!anchor_save_stack.empty() && interest_in_saving)
    {
        addToMap(anchor_map, anchor_save_stack.top(), anchor_data_save_stack.top());
        anchor_save_stack.pop();
        anchor_data_save_stack.pop();
        subtract_count = 2;
        return false;
    }
    return interest_in_saving;
}

void addInfoToDataStack(std::stack<std::string>& anchor_data, 
    std::string info)
{
    if (!anchor_data.empty())
    {
        std::string& temp = anchor_data.top();
        temp += info;
    }
    else
    {
        anchor_data.push(info);
    }
}

std::string parseLibyaml(std::string name_of_file)
{
    FILE *input;
    yaml_parser_t parser;
    yaml_event_t event;

    std::string libyaml_final_output = "";

    std::string relevant_saving_info = "";
    
    bool interest_in_saving = false;

    int subtract_count = 2;

    std::map<std::string, std::string> anchor_map;

    std::stack<std::string> anchor_save_stack;
    
    std::stack<std::string> anchor_data_save_stack;

    std::stack<char> mode_stack;

    mode_stack.push(' ');

    std::stack<bool> map_mode_stack;

    bool map_mode = true;
    
    input = fopen(name_of_file.c_str(), "rb");

    assert(input);

    if (!yaml_parser_initialize(&parser)) 
    {
        return "ERROR: Could not initialize the parser object\n";
    }
    yaml_parser_set_input_file(&parser, input);

    while (true) 
    {
        std::string local_event_output = "";

        yaml_event_type_t type;

        if (!yaml_parser_parse(&parser, &event)) 
        {
            if ( parser.problem_mark.line || parser.problem_mark.column ) 
            {
                fprintf(stderr, "Parse error: %s\nLine: %lu Column: %lu\n",
                    parser.problem,
                    (unsigned long)parser.problem_mark.line + 1,
                    (unsigned long)parser.problem_mark.column + 1);
            }
            else {
                fprintf(stderr, "Parse error: %s\n", parser.problem);
            }
            return "ERROR: Bad parsing";
        }
        type = event.type;

        switch (type)
        {
            case YAML_STREAM_END_EVENT:

            interest_in_saving = addToMapDirective(anchor_map, anchor_save_stack, 
                anchor_data_save_stack, subtract_count, interest_in_saving);

                break;
            case YAML_DOCUMENT_END_EVENT:

                interest_in_saving = addToMapDirective(anchor_map, anchor_save_stack, 
                    anchor_data_save_stack, subtract_count, interest_in_saving);

                break;
            case YAML_MAPPING_START_EVENT:
                if (!mode_stack.empty())
                {
                    positionAnalysis(local_event_output, mode_stack.top(), map_mode);
                }

                if (mode_stack.top()=='M')
                {
                    map_mode_stack.push(!map_mode);
                }

                mode_stack.push('M');

                map_mode = true;

                if (event.data.mapping_start.anchor)
                {
                    anchor_save_stack.push(((char*)event.data.mapping_start.anchor));
                    interest_in_saving = true;
                    subtract_count = 2;
                }      
                else
                {
                    local_event_output += 
                        std::string((char*)event.data.scalar.value, event.data.scalar.length);        
                }

                if (event.data.mapping_start.tag)
                {
                    std::string temp_translator = ((char*)event.data.mapping_start.tag);

                    local_event_output += temp_translator + " \n";
                }                
                else
                {
                    local_event_output += "\n";
                }

                break;
            case YAML_MAPPING_END_EVENT:
                mode_stack.pop();

                if(mode_stack.top()=='M')
                {
                    map_mode = map_mode_stack.top();
                    map_mode_stack.pop();
                }

            interest_in_saving = addToMapDirective(anchor_map, anchor_save_stack, 
                anchor_data_save_stack, subtract_count, interest_in_saving);

                break;
            case YAML_SEQUENCE_START_EVENT:
                if (!mode_stack.empty())
                {
                    map_mode = positionAnalysis(local_event_output, mode_stack.top(), map_mode);
                }

                mode_stack.push('S');

                if (event.data.sequence_start.anchor)
                if (event.data.scalar.anchor)
                {
                    anchor_save_stack.push((char*)event.data.sequence_start.anchor);
                    interest_in_saving = true;
                    subtract_count = 2;
                }
                else
                {
                    local_event_output += std::string((char*)event.data.scalar.value, event.data.scalar.length);                
                }

                if (event.data.sequence_start.tag) 
                {
                    std::string temp_translator = ((char*)event.data.sequence_start.tag);

                    local_event_output += (temp_translator + " \n");
                }
                else
                {
                    local_event_output += "\n";
                }

                break;
            case YAML_SEQUENCE_END_EVENT:
                mode_stack.pop();

                interest_in_saving = addToMapDirective(anchor_map, anchor_save_stack, 
                    anchor_data_save_stack, subtract_count, interest_in_saving);

                break;
            case YAML_SCALAR_EVENT:
                map_mode = positionAnalysis(local_event_output, mode_stack.top(), map_mode);

                if (event.data.scalar.tag)
                {
                    std::string temp_translator = ((char*)event.data.scalar.tag);

                    local_event_output += temp_translator + " - ";
                }
                else
                {
                    local_event_output += "- ";
                }

                if (event.data.scalar.anchor)
                {
                    anchor_save_stack.push((char*)event.data.scalar.anchor);
                    interest_in_saving = true;
                    subtract_count = 2;
                }
                else
                {
                    local_event_output += std::string((char*)event.data.scalar.value, event.data.scalar.length);
                    
                    local_event_output += ("\n");                   
                }

                break;
            case YAML_ALIAS_EVENT:
            {
                std::string temp_translator = ((char*) event.data.alias.anchor);
                
                std::string& temp_holder = anchor_map[temp_translator];

                if (!temp_holder.empty())
                {
                    map_mode = positionAnalysis(local_event_output, mode_stack.top(), map_mode);
                    local_event_output += "\n" + temp_holder;
                }
                else
                {
                    yaml_event_delete(&event);

                    assert(!fclose(input));

                    yaml_parser_delete(&parser);
                    return "the referenced anchor is not defined";
                }
                break;
            }
            default:                 
                break;
        }
        
        yaml_event_delete(&event);

        if (type == YAML_STREAM_END_EVENT)
            break;

        if(subtract_count <= 1 && interest_in_saving)
        {
            addInfoToDataStack(anchor_data_save_stack, local_event_output);
        }

        if(interest_in_saving)
        {
            subtract_count--;
        }

        libyaml_final_output += local_event_output;

    }

    assert(!fclose(input));

    yaml_parser_delete(&parser);

    fflush(stdout);

    return libyaml_final_output;
}

// ---------------------------------------------------------------------------------
// ------------------------------ yaml-cpp test code -------------------------------
// ---------------------------------------------------------------------------------

std::string parseYamlCppNode(YAML::Node& head)
{
    std::stack <YAML::Node> iteration_list_stack;

    std::stack <std::string> additional_info_stack;

    iteration_list_stack.push(head);
    additional_info_stack.push("U");

    std::string yamlcpp_final_output = "";

    while (!iteration_list_stack.empty())
    {
        YAML::Node base_iterator = iteration_list_stack.top();
        iteration_list_stack.pop();
        
        yamlcpp_final_output += additional_info_stack.top() + ": ";
        additional_info_stack.pop();   

        const std::string& tag_holder = base_iterator.Tag();

        if(tag_holder != "?" && tag_holder != "!")
        {
            yamlcpp_final_output += tag_holder + " ";
        }

        switch (base_iterator.Type())
        {    
            case YAML::NodeType::Null:
            {
                 yamlcpp_final_output = "";
                break;
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
    return yamlcpp_final_output;
}

// ---------------------------------------------------------------------------------
// ---------------------------------- testcode -------------------------------------
// ---------------------------------------------------------------------------------

bool compareStringsCustom(std::string compareMeOne, std::string compareMeTwo)
{
    std::string::iterator ptrOne = compareMeOne.begin();
    std::string::iterator ptrTwo = compareMeTwo.begin();

    while (*ptrOne == *ptrTwo && ((ptrOne != compareMeOne.end()) || (ptrTwo != compareMeTwo.end())))
    {
        if(*ptrOne == *ptrTwo)
        {
            std::cout << *ptrTwo;
        }
        ptrOne++;
        ptrTwo++;
    }

    if(!((ptrOne == compareMeOne.end())  && (ptrTwo == compareMeTwo.end())))
    {
        std::cout << "(X)";
        return false;
    }
    else
    {
        return true;
    }
}

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    std::cout << "----------- libyaml tests -----------" << std::endl;

    // parseLibyaml(args[1]);

    std::string libyaml_final_output = parseLibyaml(args[1]);
    std::cout << libyaml_final_output << "(END)" << std::endl;

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;

    std::string yamlcpp_final_output;
    try
    {   
        YAML::Node node = YAML::LoadFile(args[1]);
        
        // YAML::Node node = YAML::Load("[1, 2, 3]");
        std::cout << "Node type: " << node.Type() << std::endl;

        yamlcpp_final_output = parseYamlCppNode(node);
    }
    catch (const std::exception& err)
    {
        yamlcpp_final_output = err.what();
    }
    std::cout << "Four" << std::endl;


    std::cout << "--------yaml-cpp Output:" << std::endl;
    std::cout << yamlcpp_final_output << "(END)" << std::endl;
    std::cout << "--------" << std::endl;
    std::cout << "- Conclusion: " << std::endl;

    if (compareStringsCustom(libyaml_final_output, yamlcpp_final_output))
    {
        std::cout << "(END)" << std::endl;  
        std::cout << "Cases equal!" << std::endl;
    }
    else
    {
        std::cout << "(END)" << std::endl;
        std::cout << "Cases different!" << std::endl;
    }

    return 0;
}

// \0 in the middle