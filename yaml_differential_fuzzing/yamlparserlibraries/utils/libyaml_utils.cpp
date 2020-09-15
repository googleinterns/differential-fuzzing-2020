#include "libyaml_utils.h"
#include "logger.h"

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------
namespace
{
enum class mode_type 
{
    EMPTY = 0, 
    KEY_TYPE = 1, 
    VALUE_TYPE = 2, 
    SEQUENCE_TYPE = 3, 
    UNKNOWN_TYPE = 4,
    MAP_TYPE = 5
};

// Necessary for finding the current mode of the current state. Important sice
// libyaml does not tell you if a certain part of a map is a key or a value
bool FindModeType(const mode_type reference_type, const bool is_map_key, mode_type* add_to_me)
{
    if (add_to_me != nullptr)
    {
        if (reference_type ==  mode_type::MAP_TYPE)
        {
            if (is_map_key)
            {
                *add_to_me =  mode_type::KEY_TYPE;
            }
            else
            {
                *add_to_me =  mode_type::VALUE_TYPE;
            }
            return !is_map_key;
        }
        else
        {
            *add_to_me =  reference_type;
            return is_map_key;
        }
    }
    return is_map_key;
}

void AddTag(const yaml_char_t* tag, YAML::Node* current_node)
{
    if (current_node != nullptr)
    {
        if (tag)
        {
            std::string temp_tag_translator = ((char*) tag);

            current_node->SetTag(temp_tag_translator);                    
        }
        else if (current_node->Tag().empty())
        {
            current_node->SetTag("?");
        }
    }
}

void AddToNode(const yaml_char_t* tag, YAML::Node* add_to_me, YAML::Node* add_me, 
    std::stack<YAML::Node>* key_stack, const mode_type tracking_current_type = mode_type::EMPTY)
{
    AddTag(tag, add_me);

    if (tracking_current_type != mode_type::EMPTY && add_to_me != nullptr)
    {
        if (tracking_current_type ==  mode_type::SEQUENCE_TYPE && add_to_me->IsSequence())
        {
            TEST_PPRINT("squ type\n")
            add_to_me->push_back(*add_me);
        }
        else if (tracking_current_type ==  mode_type::KEY_TYPE && add_to_me->IsMap())
        {
            TEST_PPRINT("key type\n")
            key_stack->push(*add_me);
        }
        else if (tracking_current_type ==  mode_type::VALUE_TYPE && add_to_me->IsMap())
        {
            TEST_PPRINT("map type\n")
            if (!key_stack->empty())
            {
                (*add_to_me)[key_stack->top()] = *add_me;
                key_stack->pop();
            }
        }
        else
        {
            TEST_PPRINT("? type\n")
        }
    }
}

bool EndEventAddition(bool is_map_key, std::vector<YAML::Node>* libyaml_local_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, std::stack<YAML::Node>* key_stack)
{
    if (libyaml_local_output->size() > 1 && mode_stack->size() > 1)
    {
        mode_stack->pop();

        if (mode_stack->top() == mode_type::MAP_TYPE && !map_mode_stack->empty())
        {
            is_map_key = map_mode_stack->top();
            map_mode_stack->pop();
        }
        mode_type temp_position_info;

        FindModeType(mode_stack->top(), !is_map_key, &temp_position_info);

        YAML::Node temp_local_output_back = libyaml_local_output->back();

        libyaml_local_output->pop_back();

        AddToNode(nullptr, &libyaml_local_output->back(), &temp_local_output_back, key_stack, temp_position_info);
    }
    return is_map_key;
}

void ResetStateVariables(std::stack<YAML::Node>* key_stack,
    std::stack<mode_type>* mode_stack, std::stack<bool>* map_mode_stack,
    std::vector<YAML::Node>* libyaml_local_output, std::vector<YAML::Node>* libyaml_final_output,
    bool* is_map_key, std::map<std::string, YAML::Node>* anchor_map)
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

    *is_map_key = true;

    anchor_map->clear();
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

std::unique_ptr<std::vector<yaml_event_t>> GetEvents(const uint8_t* input, 
    const size_t input_size, std::string* error_message_container)
{
    std::unique_ptr<std::vector<yaml_event_t>> event_list(new std::vector<yaml_event_t>);

    yaml_parser_t parser;
    yaml_event_t event;

    if (!yaml_parser_initialize(&parser)) 
    {
        TEST_PPRINT("ERROR: Failed to initialize library parser\n");

        *error_message_container = std::string("ERROR");

        return std::move(event_list);
    }

    yaml_parser_set_input_string(&parser, input, input_size);
    
    while (true) 
    {
        yaml_event_type_t type;

        type = event.type;

        if (!yaml_parser_parse(&parser, &event))
        {
            yaml_event_delete(&event);

            TEST_PPRINT("ERROR: Libyaml library unable to parse input\n");

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

    return event_list;
}

bool IsExplicitTag(yaml_char_t* check_tag)
{
    if (check_tag != nullptr)
    {
        std::string temp_tag_translator = ((char*) check_tag);

        return temp_tag_translator != "!" && temp_tag_translator != "?";
    }
    return false;
}

void InsertIntoAnchorMap(const std::string key, YAML::Node* insert_me, 
    std::map<std::string, YAML::Node>* anchor_map)
{
    if (anchor_map->find(key) != anchor_map->end())
    {
        TEST_PPRINT("substitute\n");
        anchor_map->erase(key);
    }
    (*anchor_map)[key] = *insert_me;
}

} // anonymous namespace

std::vector<YAML::Node>* libyaml_parsing::ParseLibyaml(const uint8_t* input, 
    size_t input_size, std::string* error_message_container)
{
    std::vector<YAML::Node> libyaml_local_output;

    std::vector<YAML::Node>* libyaml_final_output = new std::vector<YAML::Node>;

    std::stack<YAML::Node> key_stack;

    std::stack<mode_type> mode_stack;

    mode_stack.push(mode_type::UNKNOWN_TYPE);

    std::stack<bool> map_mode_stack;

    bool is_map_key;
    
    std::map<std::string, YAML::Node> anchor_map;

    ResetStateVariables(&key_stack, &mode_stack, &map_mode_stack, &libyaml_local_output,
        libyaml_final_output, &is_map_key, &anchor_map);

    std::unique_ptr<std::vector<yaml_event_t>> event_list = GetEvents(input, input_size, 
        error_message_container);
    
    if (!error_message_container->empty())
    {
        delete libyaml_final_output;

        return nullptr;
    }

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

                ResetStateVariables(&key_stack, &mode_stack, &map_mode_stack, &libyaml_local_output,
                    libyaml_final_output, &is_map_key, &anchor_map);

                break;        
            case YAML_DOCUMENT_START_EVENT:
                TEST_PPRINT("DOC+\n");

                ResetStateVariables(&key_stack, &mode_stack, &map_mode_stack, &libyaml_local_output,
                    libyaml_final_output, &is_map_key, &anchor_map);

                break;

            case YAML_MAPPING_END_EVENT:
                TEST_PPRINT("MAP-\n");

                is_map_key = EndEventAddition(is_map_key, &libyaml_local_output, &mode_stack, &map_mode_stack, &key_stack);

                break;
            case YAML_SEQUENCE_END_EVENT:
                TEST_PPRINT("SQU-\n");

                is_map_key = EndEventAddition(is_map_key, &libyaml_local_output, &mode_stack, &map_mode_stack, &key_stack);

                break;
            case YAML_MAPPING_START_EVENT:
                TEST_PPRINT("MAP+\n");

                libyaml_local_output.push_back(YAML::Node(YAML::NodeType::Map));
                AddTag(event->data.mapping_start.tag, &libyaml_local_output.back());

                if (!mode_stack.empty())
                {
                    FindModeType(mode_stack.top(), is_map_key, &tracking_current_type);

                    if (mode_stack.top() ==  mode_type::MAP_TYPE)
                    {
                        map_mode_stack.push(!is_map_key);
                    }
                }

                mode_stack.push(mode_type::MAP_TYPE);
                is_map_key = true;

                if (event->data.mapping_start.anchor)
                {
                    TEST_PPRINT("ANCH-map+\n");
                    InsertIntoAnchorMap(std::string((char*)event->data.mapping_start.anchor),
                        &libyaml_local_output.back(), &anchor_map);
                }

                break;
            case YAML_SEQUENCE_START_EVENT:
                TEST_PPRINT("SQU+\n");
                libyaml_local_output.push_back(YAML::Node(YAML::NodeType::Sequence));
                AddTag(event->data.sequence_start.tag, &libyaml_local_output.back());

                if (!mode_stack.empty())
                {
                    if (mode_stack.top() ==  mode_type::MAP_TYPE)
                    {
                        map_mode_stack.push(!is_map_key);
                    }
                }

                mode_stack.push(mode_type::SEQUENCE_TYPE);

                if (event->data.sequence_start.anchor)
                {
                    TEST_PPRINT("ANCH-squ+\n");

                    InsertIntoAnchorMap(std::string((char*)event->data.sequence_start.anchor),
                        &libyaml_local_output.back(), &anchor_map);
                }

                break;

            case YAML_SCALAR_EVENT:
            {
                TEST_PPRINT("SCL\n");

                std::string temp_scalar_string = std::string((char*) event->data.scalar.value, event->data.scalar.length);

                YAML::Node add_me;
                
                if (temp_scalar_string == "~")
                {
                    add_me = YAML::Node(YAML::NodeType::Null);
                }
                else
                {
                    add_me =  YAML::Node(temp_scalar_string);
                }

                AddTag(event->data.scalar.tag, &add_me);

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

                        if (event->data.scalar.length != 0)
                        {
                            InsertIntoAnchorMap(temp_translator,
                                &add_me, &anchor_map);

                            is_map_key = FindModeType(mode_stack.top(), is_map_key, &tracking_current_type);

                            if (libyaml_local_output.empty())
                            {
                                libyaml_local_output.push_back(add_me);
                            }
                            else
                            {
                                AddToNode(event->data.scalar.tag, &libyaml_local_output.back(), 
                                    &add_me, &key_stack, tracking_current_type);
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

                            if (mode_stack.top() ==  mode_type::MAP_TYPE && !is_map_key)
                            {
                                is_map_key = FindModeType(mode_stack.top(), is_map_key, &tracking_current_type);

                                add_me = YAML::Node(YAML::NodeType::Null);

                                AddToNode(event->data.scalar.tag, &libyaml_local_output.back(), 
                                    &add_me, &key_stack, tracking_current_type);
                                
                                break;
                            }
                            mode_stack.pop();

                            if (!mode_stack.empty())
                            {
                                if (mode_stack.top() ==  mode_type::MAP_TYPE)
                                {
                                    TEST_PPRINT("map\n");
                                    is_map_key = map_mode_stack.top();
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
                    is_map_key = FindModeType(mode_stack.top(), is_map_key, &tracking_current_type);

                    if (event->data.scalar.length <= 0 && !IsExplicitTag(event->data.scalar.tag) && 
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
                        AddToNode(event->data.scalar.tag, &libyaml_local_output.back(), &add_me, &key_stack, tracking_current_type);
                    }
                }
                break;
            }
            case YAML_ALIAS_EVENT:
            {
                TEST_PPRINT("ALI\n");

                std::string temp_translator = ((char*) event->data.alias.anchor);
                
                std::map<std::string, YAML::Node>::iterator search_iterator =
                    anchor_map.find(temp_translator);

                if (search_iterator != anchor_map.end())
                {
                    if (mode_stack.empty())
                    {
                        break;
                    }

                    is_map_key = FindModeType(mode_stack.top(), is_map_key, &tracking_current_type);

                    AddToNode(nullptr, &libyaml_local_output.back(), 
                        &(search_iterator->second), &key_stack, tracking_current_type);
                }
                else
                {
                    WipeEventList(event_list.get());

                    TEST_PPRINT("ERROR: Missing anchor causing fatal structure error\n");

                    *error_message_container = std::string("ERROR");

                    delete libyaml_final_output;

                    return nullptr;
                }
                break;
            }
            default: 
                break;
        }
        TEST_PPRINT("---------\n");
    }

    WipeEventList(event_list.get());

    return libyaml_final_output;
}