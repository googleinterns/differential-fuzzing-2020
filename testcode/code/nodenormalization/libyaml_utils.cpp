#include "libyaml_utils.h"


// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

bool positionAnalysis(mode_type* add_to_me, const mode_type reference_character, const bool map_mode)
{
    if (reference_character ==  mode_type::MAP_TYPE)
    {
        if (map_mode)
        {
            *add_to_me =  mode_type::KEY_TYPE;
        }
        else
        {
            *add_to_me =  mode_type::VALUE_TYPE;
        }
        return !map_mode;
    }
    else if (reference_character ==  mode_type::SEQUENCE_TYPE)
    {
        *add_to_me =  mode_type::SEQUENCE_TYPE;
    }
    else
    {
        *add_to_me =  mode_type::UNKNOWN_TYPE;
    }

    return map_mode;
}

void addTag(YAML::Node* current_node, yaml_char_t* tag)
{
    if (tag)
    {
        std::string temp_tag_translator = ((char*)tag);

        current_node->SetTag(temp_tag_translator);                    
    }
    else if(current_node->Tag().empty())
    {
        current_node->SetTag("?");
    }
}

void addToNode
    (YAML::Node* addToMe, YAML::Node* add_me, std::stack<YAML::Node>* key_stack, 
    const mode_type* tracking_current_type, yaml_char_t* tag)
{
    addTag(add_me, tag);
    if (*tracking_current_type ==  mode_type::SEQUENCE_TYPE)
    {
        addToMe->push_back(*add_me);
    }
    else if (*tracking_current_type ==  mode_type::KEY_TYPE)
    {
        key_stack->push(*add_me);
        (*addToMe)[*add_me];
    }
    else if (*tracking_current_type ==  mode_type::VALUE_TYPE)
    {
        (*addToMe)[key_stack->top()] = *add_me;
        key_stack->pop();
    }
}

bool endEventAddition
    (std::vector<YAML::Node>* libyaml_local_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, bool map_mode, std::stack<YAML::Node>* key_stack)
{

    if (libyaml_local_output->size() > 1)
    {
        mode_stack->pop();
        
        if (mode_stack->top() ==  mode_type::MAP_TYPE)
        {
            map_mode = map_mode_stack->top();
            map_mode_stack->pop();
        }
        mode_type temp_position_info;

        positionAnalysis(&temp_position_info, (mode_stack->top()), !map_mode);

        YAML::Node temp_node = libyaml_local_output->back();

        libyaml_local_output->pop_back();

        addToNode(&libyaml_local_output->back(), &temp_node, key_stack, &temp_position_info, nullptr);
    }

    return map_mode;
}

void restartVariables (std::stack<YAML::Node>* key_stack, 
    std::stack<mode_type>* mode_stack, std::stack<bool>* map_mode_stack, 
    std::vector<YAML::Node>* libyaml_local_output, std::vector<YAML::Node>* libyaml_final_output,
    bool* map_mode, std::map<std::string, YAML::Node>* anchor_map)
{
    while (!key_stack->empty())
    {
        key_stack->pop();
    }

    while (!mode_stack->empty())
    {
        mode_stack->pop();
    };
    mode_stack->push(mode_type::UNKNOWN_TYPE);

    while (!map_mode_stack->empty())
    {
        map_mode_stack->pop();
    }

    if (!libyaml_local_output->empty())
    {
        libyaml_final_output->push_back(libyaml_local_output->back());
    }

    libyaml_local_output->clear();

    *map_mode = true;

    anchor_map->clear();
}

std::string parseLibyaml(const std::string name_of_file, std::string* error_message_container)
{
    return name_of_file;
}

std::vector<YAML::Node> normalizeLibyaml(std::string name_of_file, std::string* error_message_container)
{
    FILE *input;
    yaml_parser_t parser;
    yaml_event_t event;

    std::vector<YAML::Node> libyaml_local_output;

    std::vector<YAML::Node> libyaml_final_output;

    std::stack<YAML::Node> key_stack;

    std::stack<mode_type> mode_stack;

    mode_stack.push(mode_type::UNKNOWN_TYPE);

    std::stack<bool> map_mode_stack;

    bool map_mode = true;
    
    std::map<std::string, YAML::Node> anchor_map;
    
    input = fopen(name_of_file.c_str(), "rb");

    assert(input);

    if (!yaml_parser_initialize(&parser)) 
    {
        fprintf(stderr, "ERROR: Failed to initialize\n");

        *error_message_container = "ERROR";

        return libyaml_local_output;
    }

    yaml_parser_set_input_file(&parser, input);
    
    while (true) 
    {

        std::cout << "L" << std::endl;

        YAML::Node new_node;

        yaml_event_type_t type;

        mode_type tracking_current_type;

        if (!yaml_parser_parse(&parser, &event)) 
        {
            yaml_event_delete(&event);

            assert(!fclose(input));

            yaml_parser_delete(&parser);

            fprintf(stderr, "ERROR: Bad parsing\n");

            *error_message_container = "ERROR";

            return libyaml_local_output;
        }
        type = event.type;
        
        switch (type)
        {
            case YAML_STREAM_END_EVENT:
                std::cout << "STR-" << std::endl;
                
                break;
            case YAML_DOCUMENT_END_EVENT:

                std::cout << "DOC-" << std::endl;
                restartVariables (&key_stack, &mode_stack, &map_mode_stack, &libyaml_local_output,
                    &libyaml_final_output, &map_mode, &anchor_map);

                break;        
            case YAML_DOCUMENT_START_EVENT:

                restartVariables (&key_stack, &mode_stack, &map_mode_stack, &libyaml_local_output,
                    &libyaml_final_output, &map_mode, &anchor_map);

                break;

            case YAML_MAPPING_END_EVENT:

                std::cout << "MAP-" << std::endl;

                map_mode = endEventAddition(&libyaml_local_output, &mode_stack, &map_mode_stack, map_mode, &key_stack);

                break;
            case YAML_SEQUENCE_END_EVENT:

                std::cout << "SQU-" << std::endl;
                map_mode = endEventAddition(&libyaml_local_output, &mode_stack, &map_mode_stack, map_mode, &key_stack);

                break;
            case YAML_MAPPING_START_EVENT:
                
                std::cout << "MAP+" << std::endl;
                libyaml_local_output.push_back(YAML::Node());
                addTag(&libyaml_local_output.back(), event.data.sequence_start.tag);

                if (!mode_stack.empty())
                {
                    positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);
                }

                if (mode_stack.top() ==  mode_type::MAP_TYPE)
                {
                    map_mode_stack.push(!map_mode);
                }

                mode_stack.push(mode_type::MAP_TYPE);
                map_mode = true;

                if (event.data.mapping_start.anchor)
                {
                    std::cout << "ANCH-M" << std::endl;
                    anchor_map[std::string((char*)event.data.mapping_start.anchor)] = libyaml_local_output.back();
                }

                break;
            case YAML_SEQUENCE_START_EVENT:

                std::cout << "SQU+" << std::endl;

                libyaml_local_output.push_back(YAML::Node());
                addTag(&libyaml_local_output.back(), event.data.sequence_start.tag);

                if (mode_stack.top() ==  mode_type::MAP_TYPE)
                {
                    map_mode_stack.push(!map_mode);
                }

            
                mode_stack.push(mode_type::SEQUENCE_TYPE);

                if (event.data.sequence_start.anchor)
                {
                    std::cout << "ANCH-S" << std::endl;
                    anchor_map[std::string((char*)event.data.sequence_start.anchor)] = libyaml_local_output.back();
                }

                break;

            case YAML_SCALAR_EVENT:
            {
                std::cout << "SCL" << std::endl;
                
                YAML::Node add_me(std::string((char*)event.data.scalar.value, event.data.scalar.length));
                addTag(&add_me, event.data.scalar.tag);

                if (event.data.scalar.anchor)
                {
                    std::cout << "ANCH" << std::endl;

                    std::string temp_translator = ((char*)event.data.scalar.anchor);
                    
                    if (event.data.scalar.value)
                    {
                        std::cout << "scl" << std::endl;
                        
                        anchor_map[temp_translator] = add_me;    

                        if(event.data.scalar.length != 0)
                        {
                            map_mode = positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                            if (libyaml_local_output.empty())
                            {
                                libyaml_local_output.push_back(add_me);
                            }
                            else
                            {
                                addToNode(&libyaml_local_output.back(), &add_me, &key_stack, &tracking_current_type, 
                                    event.data.scalar.tag);
                            }
                        }
                        else
                        {
                            mode_stack.pop();

                            if (mode_stack.top() ==  mode_type::MAP_TYPE)
                            {
                                map_mode = map_mode_stack.top();
                                map_mode_stack.pop();
                            }
                            libyaml_local_output.pop_back();
                        }
                    }
                }
                else
                {
                    map_mode = positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);
                    
                    std::cout << add_me << std::endl;
                    
                    if (event.data.scalar.length <= 0 && !event.data.scalar.tag && 
                            event.data.scalar.style == YAML_PLAIN_SCALAR_STYLE)
                    {
                        add_me = YAML::Node();
                    }

                    if (libyaml_local_output.empty())
                    {
                        libyaml_local_output.push_back(add_me);
                    }            
                    else
                    {
                        addToNode(&libyaml_local_output.back(), &add_me, &key_stack, &tracking_current_type, 
                            event.data.scalar.tag);
                    }
                }

                break;
            }
            case YAML_ALIAS_EVENT:
            {
                std::cout << "ALI" << std::endl;

                std::string temp_translator = ((char*) event.data.alias.anchor);

                if(anchor_map[temp_translator])
                {
                    map_mode = positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                    addToNode(&libyaml_local_output.back(), &anchor_map[temp_translator], 
                        &key_stack, &tracking_current_type, nullptr);
                }
                else
                {
                    yaml_event_delete(&event);

                    assert(!fclose(input));

                    yaml_parser_delete(&parser);

                    fprintf(stderr, "ERROR: Missing anchor\n");

                    *error_message_container = "ERROR";

                    return libyaml_local_output;
                }
                break;
            }
            default: 
                break;
        }

        yaml_event_delete(&event);

        if (type == YAML_STREAM_END_EVENT)
        {
            break;
        }

        if (!libyaml_local_output.empty())
        {
            std::cout << "-------" << std::endl;
            std::cout << libyaml_local_output.back() << std::endl;
            std::cout << "-------" << std::endl;            
        }
    }

    assert(!fclose(input));

    yaml_parser_delete(&parser);

    fflush(stdout);
    
    std::cout << "I got to the end of the parser!" << std::endl;

    std::cout << "Size:" << libyaml_local_output.size() << std::endl;

    if (!libyaml_local_output.empty())
    {
        std::cout << "Node output:" << std::endl;
        std::cout << libyaml_local_output.back() << std::endl;
        std::cout << "-----------------" << std::endl;
        libyaml_final_output.push_back(libyaml_local_output.back());
    }

    return libyaml_final_output;
}