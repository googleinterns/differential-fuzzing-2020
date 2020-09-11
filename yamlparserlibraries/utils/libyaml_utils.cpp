#include "libyaml_utils.h"
#include "logger.h"

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------
namespace
{

bool PositionAnalysis(mode_type* add_to_me, const mode_type reference_character, const bool map_mode)
{
    if (add_to_me != nullptr)
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
    }
    return map_mode;
}

void AddTag(YAML::Node* current_node, yaml_char_t* tag)
{
    if (current_node != nullptr)
    {
        if (tag)
        {
            std::string temp_tag_translator = ((char*) tag);

            current_node->SetTag(temp_tag_translator);                    
        }
        else if(current_node->Tag().empty())
        {
            current_node->SetTag("?");
        }
    }
}

void AddToNode
    (YAML::Node* addToMe, YAML::Node* add_me, std::stack<YAML::Node>* key_stack, 
    const mode_type* tracking_current_type, yaml_char_t* tag)
{
    AddTag(add_me, tag);
    
    if (tracking_current_type != nullptr && addToMe != nullptr)
    {
        if (*tracking_current_type ==  mode_type::SEQUENCE_TYPE && addToMe->IsSequence())
        {
            TEST_PPRINT("squ type\n")
            addToMe->push_back(*add_me);
        }
        else if (*tracking_current_type ==  mode_type::KEY_TYPE && addToMe->IsMap())
        {
            TEST_PPRINT("key type\n")
            key_stack->push(*add_me);
            (*addToMe)[*add_me];
        }
        else if (*tracking_current_type ==  mode_type::VALUE_TYPE && addToMe->IsMap())
        {
            TEST_PPRINT("map type\n")
            if (!key_stack->empty())
            {
                (*addToMe)[key_stack->top()] = *add_me;
                key_stack->pop();
            }
        }
        else
        {
            TEST_PPRINT("? type\n")
        }
    }
}

bool EndEventAddition
    (std::vector<YAML::Node>* libyaml_local_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, bool map_mode, std::stack<YAML::Node>* key_stack)
{
    if (libyaml_local_output->size() > 1 && mode_stack->size() > 1)
    {
        mode_stack->pop();

        if (mode_stack->top() == mode_type::MAP_TYPE && !map_mode_stack->empty())
        {
            map_mode = map_mode_stack->top();
            map_mode_stack->pop();
        }
        mode_type temp_position_info;

        if (mode_stack->empty())
        {
            return map_mode;
        }
        PositionAnalysis(&temp_position_info, (mode_stack->top()), !map_mode);

        YAML::Node temp_node = libyaml_local_output->back();

        libyaml_local_output->pop_back();

        AddToNode(&libyaml_local_output->back(), &temp_node, key_stack, &temp_position_info, nullptr);
    }
    return map_mode;
}

void RestartVariables (std::stack<YAML::Node>* key_stack,
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
    }
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

std::unique_ptr<std::vector<yaml_event_t>> GetEvents
    (const uint8_t* input, size_t input_size, std::string* error_message_container)
{
    std::unique_ptr<std::vector<yaml_event_t>> event_list(new std::vector<yaml_event_t>);

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser)) 
    {
        TEST_PPRINT("ERROR: Failed to initialize\n");

        *error_message_container = std::string("ERROR");

        return std::move(event_list);
    }

    yaml_parser_set_input_string(&parser, input, input_size);
    
    int done = 0;

    while (true) 
    {
        yaml_event_type_t type;

        type = event.type;

        if (!yaml_parser_parse(&parser, &event))
        {
            yaml_event_delete(&event);

            TEST_PPRINT("ERROR: Bad parsing\n");

            *error_message_container = std::string("ERROR");

            WipeEventList(event_list.get());

            break;
        }

        event_list.get()->push_back(event);

        if (type == YAML_STREAM_END_EVENT)
        {
            break;
        }
    }

    yaml_parser_delete(&parser);

    fflush(stdout);

    return std::move(event_list);
}

void WipeEventList(std::vector<yaml_event_t>* event_list)
{
    for (std::vector<yaml_event_t>::iterator it = event_list->begin();
        it != event_list->end(); ++ it)
    {
        yaml_event_delete(&(*it));
    }
    event_list->clear();
}

bool RelevantTag(yaml_char_t* check_tag)
{
    if (check_tag != nullptr)
    {
        std::string temp_tag_translator = ((char*) check_tag);

        return temp_tag_translator != "!" && temp_tag_translator != "?";
    }
    return false;
}

}

std::vector<YAML::Node>* libyaml_parsing::ParseLibyaml
    (const uint8_t* input, size_t input_size, std::string* error_message_container)
{
    std::vector<YAML::Node> libyaml_local_output;

    std::vector<YAML::Node>* libyaml_final_output = new std::vector<YAML::Node>;

    std::stack<YAML::Node> key_stack;

    std::stack<mode_type> mode_stack;

    mode_stack.push(mode_type::UNKNOWN_TYPE);

    std::stack<bool> map_mode_stack;

    bool map_mode = true;
    
    std::map<std::string, YAML::Node> anchor_map;

    std::unique_ptr<std::vector<yaml_event_t>> event_list = 
        std::move(GetEvents(input, input_size, error_message_container));
    
    for (std::vector<yaml_event_t>::iterator event = event_list.get()->begin();
        event != event_list.get()->end(); ++ event) 
    {
        mode_type tracking_current_type;
        
        switch (event->type)
        {
            case YAML_STREAM_END_EVENT:
                TEST_PPRINT("STR-\n");

                break;
            case YAML_DOCUMENT_END_EVENT:
                TEST_PPRINT("DOC-\n");

                RestartVariables(&key_stack, &mode_stack, &map_mode_stack, &libyaml_local_output,
                    libyaml_final_output, &map_mode, &anchor_map);

                break;        
            case YAML_DOCUMENT_START_EVENT:
                TEST_PPRINT("DOC+\n");

                RestartVariables(&key_stack, &mode_stack, &map_mode_stack, &libyaml_local_output,
                    libyaml_final_output, &map_mode, &anchor_map);

                break;

            case YAML_MAPPING_END_EVENT:
                TEST_PPRINT("MAP-\n");

                map_mode = EndEventAddition(&libyaml_local_output, &mode_stack, &map_mode_stack, map_mode, &key_stack);

                break;
            case YAML_SEQUENCE_END_EVENT:
                TEST_PPRINT("SQU-\n");

                map_mode = EndEventAddition(&libyaml_local_output, &mode_stack, &map_mode_stack, map_mode, &key_stack);

                break;
            case YAML_MAPPING_START_EVENT:
                TEST_PPRINT("MAP+\n");

                libyaml_local_output.push_back(YAML::Node(YAML::NodeType::Map));
                AddTag(&libyaml_local_output.back(), event->data.sequence_start.tag);

                if (!mode_stack.empty())
                {
                    PositionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                    if (mode_stack.top() ==  mode_type::MAP_TYPE)
                    {
                        map_mode_stack.push(!map_mode);
                    }
                }

                mode_stack.push(mode_type::MAP_TYPE);
                map_mode = true;

                if (event->data.mapping_start.anchor)
                {
                    TEST_PPRINT("ANCH-map+\n");
                    anchor_map[std::string((char*)event->data.mapping_start.anchor)] = libyaml_local_output.back();
                }

                break;
            case YAML_SEQUENCE_START_EVENT:
                TEST_PPRINT("SQU+\n");
                libyaml_local_output.push_back(YAML::Node(YAML::NodeType::Sequence));
                AddTag(&libyaml_local_output.back(), event->data.sequence_start.tag);

                if (!mode_stack.empty())
                {
                    if (mode_stack.top() ==  mode_type::MAP_TYPE)
                    {
                        map_mode_stack.push(!map_mode);
                    }
                }

                mode_stack.push(mode_type::SEQUENCE_TYPE);

                if (event->data.sequence_start.anchor)
                {
                    TEST_PPRINT("ANCH-squ+\n");

                    anchor_map[std::string((char*)event->data.sequence_start.anchor)] = libyaml_local_output.back();
                }

                break;

            case YAML_SCALAR_EVENT:
            {
                TEST_PPRINT("SCL\n");

                YAML::Node add_me(std::string((char*)event->data.scalar.value, event->data.scalar.length));
                AddTag(&add_me, event->data.scalar.tag);

                if (event->data.scalar.anchor)
                {
                    TEST_PPRINT("ANCH-scl\n");
                    std::string temp_translator = ((char*)event->data.scalar.anchor);
                    if (mode_stack.empty())
                    {
                        break;
                    }

                    if (event->data.scalar.value)
                    {
                        TEST_PPRINT("value\n");

                        if(event->data.scalar.length != 0)
                        {
                            anchor_map[temp_translator] = add_me;
                            
                            map_mode = PositionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                            if (libyaml_local_output.empty())
                            {
                                libyaml_local_output.push_back(add_me);

                            }
                            else
                            {
                                AddToNode(&libyaml_local_output.back(), &add_me, &key_stack, &tracking_current_type, 
                                    event->data.scalar.tag);
                            }
                        }
                        else
                        {
                            TEST_PPRINT("empty\n");

                            if (mode_stack.top() ==  mode_type::SEQUENCE_TYPE)
                            {
                                TEST_PPRINT("sequence\n");
                                add_me = YAML::Node(YAML::NodeType::Null);

                                if (!libyaml_local_output.empty())
                                {
                                    if (libyaml_local_output.back().IsSequence())
                                    {
                                        TEST_PPRINT("insert\n");
                                        libyaml_local_output.back().push_back(add_me);
                                    }
                                }

                                break;
                            }

                            if (mode_stack.top() ==  mode_type::MAP_TYPE && !map_mode)
                            {
                                map_mode = PositionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                                add_me = YAML::Node(YAML::NodeType::Null);

                                AddToNode(&libyaml_local_output.back(), &add_me, &key_stack, &tracking_current_type, 
                                    event->data.scalar.tag);
                                
                                break;
                            }
                            mode_stack.pop();

                            if (!mode_stack.empty())
                            {
                                if (mode_stack.top() ==  mode_type::MAP_TYPE)
                                {
                                    TEST_PPRINT("map\n");
                                    map_mode = map_mode_stack.top();
                                    map_mode_stack.pop();
                                }

                                if (!libyaml_local_output.empty())
                                {
                                    TEST_PPRINT("pop\n");
                                    libyaml_local_output.pop_back();
                                }
                            }
                            else
                            {
                                TEST_PPRINT("insert\n");
                                libyaml_local_output.push_back(YAML::Node());
                            }
                        }
                    }
                }
                else
                {
                    TEST_PPRINT("normal\n");
                    if (mode_stack.empty())
                    {
                        break;
                    }
                    map_mode = PositionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                    if (event->data.scalar.length <= 0 && !RelevantTag(event->data.scalar.tag) && 
                            event->data.scalar.style == YAML_PLAIN_SCALAR_STYLE)
                    {
                        TEST_PPRINT("Begin from nothing\n");
                        add_me = YAML::Node();
                    }

                    if (libyaml_local_output.empty())
                    {
                        libyaml_local_output.push_back(add_me);
                    }            
                    else
                    {
                        TEST_PPRINT("Add to node\n");
                        AddToNode(&libyaml_local_output.back(), &add_me, &key_stack, &tracking_current_type, 
                            event->data.scalar.tag);
                    }
                }

                break;
            }
            case YAML_ALIAS_EVENT:
            {
                TEST_PPRINT("ALI\n");

                std::string temp_translator = ((char*) event->data.alias.anchor);
                
                if(anchor_map.find(temp_translator) != anchor_map.end())
                {
                    if (mode_stack.empty())
                    {
                        break;
                    }

                    map_mode = PositionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                    AddToNode(&libyaml_local_output.back(), &anchor_map[temp_translator], 
                        &key_stack, &tracking_current_type, nullptr);
                }
                else
                {
                    WipeEventList(event_list.get());

                    TEST_PPRINT("ERROR: Missing anchor\n");

                    *error_message_container = std::string("ERROR");

                    return libyaml_final_output;
                }
                break;
            }
            default: 
                break;
        }
        // if (!libyaml_local_output.empty())
        // {
        //     std::cout << "------------------" << std::endl;
        //     std::cout << libyaml_local_output.back() << std::endl;
        //     std::cout << "------------------" << std::endl;
        // }
        TEST_PPRINT("---------\n");
    }

    WipeEventList(event_list.get());

    fflush(stdout);

    return libyaml_final_output;
}