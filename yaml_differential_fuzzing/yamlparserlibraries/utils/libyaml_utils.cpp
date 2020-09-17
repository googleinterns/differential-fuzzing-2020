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
    MAP_TYPE = 4,
    UNKNOWN_TYPE = 5
};

enum class function_status 
{
    FAIL = 6,
    SUCCESS = 7
};

// Necessary for finding the current mode of the current state. Important since
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

function_status AddToNode(const yaml_char_t* tag, YAML::Node* add_to_me, YAML::Node* add_me, 
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

            bool no_key_associated_with_value = !key_stack->empty();
            if (key_stack->empty())
            {
                return function_status::FAIL;
            }

            (*add_to_me)[key_stack->top()] = *add_me;
            key_stack->pop();
        }
        else
        {
            return function_status::FAIL;
        }
    }
    return function_status::SUCCESS;
}

bool EndEventAddition(bool is_map_key, std::vector<YAML::Node>* libyaml_local_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, std::stack<YAML::Node>* key_stack, function_status* addition_to_node_check)
{
    if (mode_stack->size() <= 1)
    {
        *addition_to_node_check = function_status::FAIL;
        return is_map_key;
    }

    if (libyaml_local_output->size() > 1)
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

        *addition_to_node_check = AddToNode(nullptr, &libyaml_local_output->back(), &temp_local_output_back, key_stack, temp_position_info);
    }
    else
    {
        *addition_to_node_check = function_status::SUCCESS;
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

        if (!yaml_parser_parse(&parser, &event))
        {
            yaml_event_delete(&event);

            TEST_PPRINT("ERROR: Libyaml library unable to parse input\n");

            *error_message_container = std::string("ERROR");

            WipeEventList(event_list.get());

            break;
        }

        type = event.type;

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

// Parses case where we are in a scalar that is an anchor, but the
// value of the anchor is empty
function_status ParseEmptyAnchorScalar(const std::vector<yaml_event_t>::iterator event, std::stack<YAML::Node>* key_stack,
    std::stack<mode_type>* mode_stack, std::stack<bool>* map_mode_stack,
    std::vector<YAML::Node>* libyaml_local_output,
    bool* is_map_key, YAML::Node* add_me, mode_type* tracking_current_type)
{
    TEST_PPRINT("empty\n");

    if (mode_stack->top() ==  mode_type::SEQUENCE_TYPE)
    {
        TEST_PPRINT("sequence\n");
        *add_me = YAML::Node(YAML::NodeType::Null);

        if (!libyaml_local_output->empty())
        {
            TEST_PPRINT("insert\n");
            libyaml_local_output->back().push_back(*add_me);
        }
        return function_status::SUCCESS;
    }

    if (mode_stack->top() ==  mode_type::MAP_TYPE && !*is_map_key)
    {
        *is_map_key = FindModeType(mode_stack->top(), *is_map_key, tracking_current_type);

        *add_me = YAML::Node(YAML::NodeType::Null);

        return AddToNode(event->data.scalar.tag, &libyaml_local_output->back(), 
            add_me, key_stack, *tracking_current_type);
    }
    mode_stack->pop();

    if (!mode_stack->empty())
    {
        // Handle '&anchor: value' cases 
        if (mode_stack->top() ==  mode_type::MAP_TYPE)
        {
            TEST_PPRINT("map\n");
            *is_map_key = map_mode_stack->top();
            map_mode_stack->pop();
        }

        // Handle case of anchor with empty value in a sequence, or
        // when there is a dangling empty scalar
        if (!libyaml_local_output->empty())
        {
            TEST_PPRINT("pop\n");
            libyaml_local_output->pop_back();
        }
    }
    else
    {
        // Inserts the empty node
        TEST_PPRINT("insert\n");
        libyaml_local_output->push_back(YAML::Node());
    }
    return function_status::SUCCESS;
}

function_status ParseAnchorScalarContainingData(const std::vector<yaml_event_t>::iterator event, std::stack<YAML::Node>* key_stack,
    std::stack<mode_type>* mode_stack, std::vector<YAML::Node>* libyaml_local_output,
    bool* is_map_key, std::map<std::string, YAML::Node>* anchor_map, YAML::Node* add_me, 
    mode_type* tracking_current_type, std::string* temp_translator)
{
    InsertIntoAnchorMap(*temp_translator, add_me, anchor_map);

    *is_map_key = FindModeType(mode_stack->top(), *is_map_key, tracking_current_type);

    if (libyaml_local_output->empty())
    {
        libyaml_local_output->push_back(*add_me);
    }
    else
    {
        return AddToNode(event->data.scalar.tag, &libyaml_local_output->back(), 
            add_me, key_stack, *tracking_current_type);
    }
    return function_status::SUCCESS;
}

// handles cases where the scalar is an anchor
function_status ParseAnchorScalar(const std::vector<yaml_event_t>::iterator event, std::stack<YAML::Node>* key_stack,
    std::stack<mode_type>* mode_stack, std::stack<bool>* map_mode_stack,
    std::vector<YAML::Node>* libyaml_local_output,
    bool* is_map_key, std::map<std::string, YAML::Node>* anchor_map, YAML::Node* add_me, 
    mode_type* tracking_current_type)
{
    TEST_PPRINT("ANCH-scl\n");

    std::string temp_translator = ((char*) event->data.scalar.anchor);

    if (mode_stack->empty())
    {
        return function_status::SUCCESS;
    }

    if (event->data.scalar.value)
    {
        TEST_PPRINT("value\n");

        if (event->data.scalar.length != 0)
        {
            return ParseAnchorScalarContainingData(event, key_stack, mode_stack, libyaml_local_output, 
                is_map_key, anchor_map, add_me, tracking_current_type, &temp_translator);
        }
        else
        {
            return ParseEmptyAnchorScalar(event, key_stack, mode_stack, map_mode_stack,
                libyaml_local_output, is_map_key, add_me, tracking_current_type);
        }
    }
    return function_status::SUCCESS;
}

// Parses scalar case where we only have to deal with the scalar itself
function_status ParseNormalScalar(const std::vector<yaml_event_t>::iterator event, std::stack<YAML::Node>* key_stack,
    std::stack<mode_type>* mode_stack, std::vector<YAML::Node>* libyaml_local_output,
    bool* is_map_key, YAML::Node* add_me, mode_type* tracking_current_type)
{
    TEST_PPRINT("normal\n");
    if (mode_stack->empty())
    {
        return function_status::SUCCESS;;
    }
    *is_map_key = FindModeType(mode_stack->top(), *is_map_key, tracking_current_type);

    if (event->data.scalar.length <= 0 && !IsExplicitTag(event->data.scalar.tag) && 
            event->data.scalar.style == YAML_PLAIN_SCALAR_STYLE)
    {
        TEST_PPRINT("Begin from nothing\n");
        *add_me = YAML::Node();
    }

    if (libyaml_local_output->empty())
    {
        libyaml_local_output->push_back(*add_me);
    }            
    else
    {
        TEST_PPRINT("Add to node\n");
        return AddToNode(event->data.scalar.tag, &libyaml_local_output->back(), add_me, key_stack, *tracking_current_type);
    }
    return function_status::SUCCESS;;
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
                // - Update is_map_key to add information
                // - Removes info from map_mode_stack relative to the mapping event
                TEST_PPRINT("MAP-\n");

                function_status addition_to_node_success_map;

                is_map_key = EndEventAddition(is_map_key, &libyaml_local_output, &mode_stack, &map_mode_stack, &key_stack,
                    &addition_to_node_success_map);

                if (addition_to_node_success_map != function_status::SUCCESS)
                {
                    WipeEventList(event_list.get());

                    TEST_PPRINT("ERROR: Failed to insert to node\n");

                    *error_message_container = std::string("ERROR");

                    delete libyaml_final_output;

                    return nullptr;
                }

                break;
            case YAML_SEQUENCE_END_EVENT:
                // - Update is_map_key to add information
                // - Removes info from map_mode_stack relative to the mapping event
                TEST_PPRINT("SQU-\n");

                function_status addition_to_node_success_sequence;

                is_map_key = EndEventAddition(is_map_key, &libyaml_local_output, &mode_stack, &map_mode_stack, &key_stack,
                    &addition_to_node_success_sequence);

                if (addition_to_node_success_sequence != function_status::SUCCESS)
                {
                    WipeEventList(event_list.get());

                    TEST_PPRINT("ERROR: Failed to insert to node\n");

                    *error_message_container = std::string("ERROR");

                    delete libyaml_final_output;

                    return nullptr;
                }

                break;
            case YAML_MAPPING_START_EVENT:
                // - Update is_map_key to add information to use in the future to add
                // cases
                // - Saves info to map_mode_stack if relevant
                TEST_PPRINT("MAP+\n");

                libyaml_local_output.push_back(YAML::Node(YAML::NodeType::Map));
                AddTag(event->data.mapping_start.tag, &libyaml_local_output.back());

                // Safeguard against all previous modes popped from anchors
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
                // - Update is_map_key to add information to use in the future to add
                // cases
                // - Saves info to map_mode_stack if relevant
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
                // - Use is_map_key and update if necessary
                // - Saves info to map_mode_stack if relevant, and may use it to add info
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

                function_status addition_to_node_success = function_status::SUCCESS;

                if (event->data.scalar.anchor)
                {
                    addition_to_node_success = ParseAnchorScalar(event, &key_stack, &mode_stack, &map_mode_stack,
                        &libyaml_local_output, &is_map_key, &anchor_map, &add_me, 
                        &tracking_current_type);
                }
                else
                {
                    addition_to_node_success = ParseNormalScalar(event, &key_stack, &mode_stack, &libyaml_local_output,
                        &is_map_key, &add_me, &tracking_current_type);
                }

                if (addition_to_node_success != 
                    function_status::SUCCESS)
                {
                    WipeEventList(event_list.get());

                    TEST_PPRINT("ERROR: Failed to insert to node\n");

                    *error_message_container = std::string("ERROR");

                    delete libyaml_final_output;

                    return nullptr;
                }

                break;
            }
            case YAML_ALIAS_EVENT:
            {
                // - Use is_map_key and update if necessary
                // - Saves info to map_mode_stack if relevant, and may use it to add info
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

                    if (AddToNode(nullptr, &libyaml_local_output.back(), 
                         &(search_iterator->second), &key_stack, tracking_current_type) != 
                         function_status::SUCCESS)
                    {
                        WipeEventList(event_list.get());

                        TEST_PPRINT("ERROR: Failed to insert to node\n");

                        *error_message_container = std::string("ERROR");

                        delete libyaml_final_output;

                        return nullptr;
                    }
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
                TEST_PPRINT("default\n");
                break;
        }
        TEST_PPRINT("---------\n");
    }

    WipeEventList(event_list.get());

    return libyaml_final_output;
}