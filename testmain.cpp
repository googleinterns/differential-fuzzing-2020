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

std::string positionAnalysis(char reference_character, bool map_mode)
{
    if (reference_character == 'M')
    {
        if (map_mode)
        {
            return "K:";
        }
        else
        {
            return "V:";
        }
        map_mode = !map_mode;
    }
    else if (reference_character == 'S')
    {
        return "L:";
    }
    else
    {
        return "U:";
    }
    
}

void addToMap(std::map<std::string, std::string> & anchor_map, 
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
    
    int flow = -1; 
    int i = 0;
    int foundfile = 0;

    std::string libyaml_final_output = "";

    std::map<std::string, std::string> anchor_map;

    std::stack<std::string> anchor_save_stack;

    std::stack<std::string> anchor_data_save_stack;

    std::stack<char> mode_stack;

    bool map_mode = true;
    
    input = fopen(name_of_file.c_str(), "rb");
    foundfile = 1;

    if (!foundfile) 
    {
        input = stdin;
    }
    assert(input);

    if (!yaml_parser_initialize(&parser)) 
    {
        return "ERROR: Could not initialize the parser object\n";
    }
    yaml_parser_set_input_file(&parser, input);

    while (1) 
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
            case YAML_NO_EVENT:
                local_event_output += ("???\n");

                break;
            case YAML_STREAM_START_EVENT:
                break;
            case YAML_STREAM_END_EVENT:
                if (!anchor_save_stack.empty())
                {
                    addToMap(anchor_map, anchor_save_stack.top(), anchor_data_save_stack.top());
                    anchor_save_stack.pop();
                    anchor_data_save_stack.pop();
                }

                break;
            case YAML_DOCUMENT_START_EVENT:
                if (!event.data.document_start.implicit)
                    local_event_output += (" ---");

                break;
            case YAML_DOCUMENT_END_EVENT:
                if (!event.data.document_end.implicit)
                    local_event_output += (" ...");

                if (!anchor_save_stack.empty())
                {
                    addToMap(anchor_map, anchor_save_stack.top(), anchor_data_save_stack.top());
                    anchor_save_stack.pop();
                    anchor_data_save_stack.pop(); 
                }

                break;
            case YAML_MAPPING_START_EVENT:
                if(!mode_stack.empty())
                {
                    local_event_output += positionAnalysis(mode_stack.top(), map_mode) + "\n";
                }

                mode_stack.push('M');

                map_mode = true;

                if (flow == 0 && event.data.mapping_start.style == YAML_FLOW_MAPPING_STYLE)
                    local_event_output += (" {}");

                else if (flow == 1)
                    local_event_output += (" {}");

                if (event.data.mapping_start.anchor)
                {
                    anchor_save_stack.push(((char*)event.data.mapping_start.anchor));
                }       
                if (event.data.mapping_start.tag)
                {
                    std::string temp_translator = ((char*)event.data.mapping_start.tag);

                    local_event_output += " " + temp_translator + " - ";
                }                

                break;
            case YAML_MAPPING_END_EVENT:
                mode_stack.pop();

                if (!anchor_save_stack.empty())
                {
                    addToMap(anchor_map, anchor_save_stack.top(), anchor_data_save_stack.top());
                    anchor_save_stack.pop();
                    anchor_data_save_stack.pop();
                }

                break;
            case YAML_SEQUENCE_START_EVENT:
                if(!mode_stack.empty())
                {
                    local_event_output += positionAnalysis(mode_stack.top(), map_mode) + "\n";
                }

                map_mode != map_mode;

                mode_stack.push('S');

                if (flow == 0 && event.data.sequence_start.style == YAML_FLOW_SEQUENCE_STYLE)
                    local_event_output += (" []");

                else if (flow == 1)
                    local_event_output += (" []");

                if (event.data.sequence_start.anchor)
                if (event.data.scalar.anchor)
                {
                    anchor_save_stack.push((char*)event.data.sequence_start.anchor);
                }
                if (event.data.sequence_start.tag) 
                {
                    std::string temp_translator = ((char*)event.data.sequence_start.tag);

                    local_event_output += (" " + temp_translator + " - ");
                }

                break;
            case YAML_SEQUENCE_END_EVENT:
                mode_stack.pop();

                if (!anchor_save_stack.empty())
                {
                    addToMap(anchor_map, anchor_save_stack.top(), anchor_data_save_stack.top());
                    anchor_save_stack.pop();
                    anchor_data_save_stack.pop();
                }

                break;
            case YAML_SCALAR_EVENT:
                local_event_output += positionAnalysis(mode_stack.top(), map_mode);

                map_mode != map_mode;

                if (event.data.scalar.tag)
                {
                    std::string temp_translator = ((char*)event.data.scalar.tag);

                    local_event_output += " " + temp_translator + " - ";
                }
                else
                {
                    local_event_output += " - ";
                }

                if (event.data.scalar.anchor)
                {
                    anchor_save_stack.push((char*)event.data.scalar.anchor);
                }


                local_event_output += std::string((char*)event.data.scalar.value, event.data.scalar.length);
                
                local_event_output += ("\n");

                break;
            case YAML_ALIAS_EVENT:
            {
                std::string temp_translator = ((char*) event.data.alias.anchor);

                local_event_output += positionAnalysis(mode_stack.top(), map_mode) + " - " + temp_translator + "\n";
                
                map_mode != map_mode;

                std::string& temp_holder = anchor_map[temp_translator];

                if (!temp_holder.empty())
                {
                    local_event_output += temp_holder;
                }
                else
                {
                    local_event_output += "ERROR";
                }
                break;
            }
            default:
                abort();
        }
        
        yaml_event_delete(&event);

        if (type == YAML_STREAM_END_EVENT)
            break;

        addInfoToDataStack(anchor_data_save_stack, local_event_output);
        
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
    additional_info_stack.push("s");

    std::string yamlcpp_final_output = "";

    int key_counter = 1;

    while (!iteration_list_stack.empty())
    {
        YAML::Node base_iterator = iteration_list_stack.top();
        iteration_list_stack.pop();

        yamlcpp_final_output += additional_info_stack.top() + ": ";
        additional_info_stack.pop();


        // Processes tags:
        const std::string& tag_holder = base_iterator.Tag();

        if(tag_holder != "?" && tag_holder != "!")
        {
            yamlcpp_final_output += tag_holder + " ";
        }

        switch (base_iterator.Type())
        {    
            case YAML::NodeType::Null:
            {
                yamlcpp_final_output += "- Null case";
                break;
            }
            case YAML::NodeType::Scalar:
            {
                // if(map_mode)
                // {
                //     if(key_counter%2)
                //     {
                //         yamlcpp_final_output += "K";
                //     }
                //     else
                //     {
                //         yamlcpp_final_output += "V";
                        
                //     }
                // }
                // else
                // {
                //     yamlcpp_final_output += "V";
                // }
                
                // key_counter--;

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
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    std::cout << "----------- libyaml tests -----------" << std::endl;

    // parseLibyaml(args[1]);

    std::string libyaml_final_output = parseLibyaml(args[1]);
    std::cout << libyaml_final_output << std::endl;

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
        std::cout << err.what() << std::endl;
    }

    std::cout << "--------yaml-cpp Output:" << std::endl;
    std::cout << yamlcpp_final_output << std::endl;
    std::cout << "--------" << std::endl;

    return 0;
}

// \0 in the middle