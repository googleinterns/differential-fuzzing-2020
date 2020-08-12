#include "utils.h"

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
        std::cout << "Add tag" << std::endl;
        std::string temp_tag_translator = ((char*)tag);

        current_node->SetTag(temp_tag_translator);                    
    }
    else
    {
        current_node->SetTag("?");
    }
}

void addToNode
    (YAML::Node* addToMe, YAML::Node* addMe, std::stack<YAML::Node>* key_stack, 
    const mode_type* tracking_current_type, yaml_char_t* tag)
{
    addTag(addMe, tag);
    if (*tracking_current_type ==  mode_type::SEQUENCE_TYPE)
    {
        addToMe->push_back(*addMe);
    }
    else if (*tracking_current_type ==  mode_type::KEY_TYPE)
    {
        key_stack->push(*addMe);
        (*addToMe)[*addMe];
    }
    else if (*tracking_current_type ==  mode_type::VALUE_TYPE)
    {
        (*addToMe)[key_stack->top()] = *addMe;
        key_stack->pop();
    }
    else
    {
        std::cout << "no case" << std::endl;
    }
    
}

// map_mode = end_event_addition(&libyaml_final_output, &mode_stack, &map_mode_stack, !map_mode, &key_stack);
bool end_event_addition
    (std::vector<YAML::Node>* libyaml_final_output, std::stack<mode_type>* mode_stack, 
    std::stack<bool>* map_mode_stack, bool map_mode, std::stack<YAML::Node>* key_stack)
{

    mode_stack->pop();
    
    if (mode_stack->top() ==  mode_type::MAP_TYPE)
    {
        map_mode = map_mode_stack->top();
        map_mode_stack->pop();
    }

    if (libyaml_final_output->size() > 1)
    {
        mode_type temp_position_info;

        positionAnalysis(&temp_position_info, (mode_stack->top()), !map_mode);

        YAML::Node temp_node = libyaml_final_output->back();

        libyaml_final_output->pop_back();

        addToNode(&libyaml_final_output->back(), &temp_node, key_stack, &temp_position_info, nullptr);
    }

    return map_mode;
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

        return libyaml_final_output;
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

            return libyaml_final_output;
        }
        type = event.type;
        
        switch (type)
        {
            case YAML_STREAM_END_EVENT:
                
                std::cout << "end event" << std::endl;
                
                break;
            case YAML_DOCUMENT_END_EVENT:

                std::cout << "Endoc" << std::endl;

                break;            
            case YAML_DOCUMENT_START_EVENT:

                std::cout << "Startdoc" << std::endl;

                break;

            case YAML_MAPPING_END_EVENT:

                std::cout << "SQU-" << std::endl;

                map_mode = end_event_addition(&libyaml_final_output, &mode_stack, &map_mode_stack, map_mode, &key_stack);

                break;
            case YAML_SEQUENCE_END_EVENT:

                std::cout << "SQU-" << std::endl;
                map_mode = end_event_addition(&libyaml_final_output, &mode_stack, &map_mode_stack, map_mode, &key_stack);

                if (event.data.scalar.anchor)
                {
                    anchor_map[std::string((char*)event.data.scalar.anchor)] = libyaml_final_output.back();
                }

                break;
            case YAML_MAPPING_START_EVENT:
                
                std::cout << "MAP+" << std::endl;
                libyaml_final_output.push_back(YAML::Node());

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
                    anchor_map[std::string((char*)event.data.mapping_start.anchor)] = libyaml_final_output.back();
                }

                if (event.data.mapping_start.tag)
                {
                    //more mapps cases
                }                

                break;
            case YAML_SEQUENCE_START_EVENT:
                std::cout << "SQU+" << std::endl;

                libyaml_final_output.push_back(YAML::Node());

                if (mode_stack.top() ==  mode_type::MAP_TYPE)
                {
                    map_mode_stack.push(!map_mode);
                }

            
                mode_stack.push( mode_type::SEQUENCE_TYPE);

                // if (event.data.sequence_start.anchor)
                // {
                //     //handle anchors
                // }      
                
                if (event.data.sequence_start.tag) 
                {
                    //more squences cases
                }

                break;

            case YAML_SCALAR_EVENT:
            {
                std::cout << "SCL" << std::endl;
                map_mode = positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                YAML::Node addMe(std::string((char*)event.data.scalar.value, event.data.scalar.length));

                if (event.data.scalar.length <= 0)
                {
                    yaml_event_delete(&event);

                    assert(!fclose(input));

                    yaml_parser_delete(&parser);

                    fprintf(stderr, "ERROR: Empty node\n");

                    *error_message_container = "ERROR";

                    return libyaml_final_output;
                }

                if (libyaml_final_output.empty())
                {
                    libyaml_final_output.push_back(addMe);
                }            
                else
                {
                    // addTag(&addMe, event.data.scalar.tag);
                    addToNode(&libyaml_final_output.back(), &addMe, &key_stack, &tracking_current_type, 
                        event.data.scalar.tag);
                }           

                if (event.data.scalar.anchor)
                {
                    std::string temp_translator = ((char*)event.data.scalar.anchor);

                    
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
                    
                    // std::cout << anchor_map[temp_translator] << std::endl;

                    addToNode(&libyaml_final_output.back(), &anchor_map[temp_translator], 
                        &key_stack, &tracking_current_type, nullptr);
                }
                else
                {
                    yaml_event_delete(&event);

                    assert(!fclose(input));

                    yaml_parser_delete(&parser);

                    fprintf(stderr, "ERROR: Missing anchor\n");

                    *error_message_container = "ERROR";

                    return libyaml_final_output;                    
                }
                

                // if ((libyaml_final_output.back())[temp_translator])
                // {
                //     // UNTESTED
                //     map_mode = positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                //     YAML::Node temp = (libyaml_final_output.back())[temp_translator];

                //     addToNode(&libyaml_final_output.back(), &temp, &key_stack, &tracking_current_type,
                //         event.data.scalar.tag);
                // }
                // else
                // {

                // }
                break;
            }
            default: 

                break;
        }
        
        yaml_event_delete(&event);

        if (type == YAML_STREAM_END_EVENT)
            break;
    }

    assert(!fclose(input));

    yaml_parser_delete(&parser);

    fflush(stdout);
    
    std::cout << "I got to the end of the parser!" << std::endl;

    std::cout << "Size:" << libyaml_final_output.size() << std::endl;

    if (!libyaml_final_output.empty())
    {
        std::cout << "Node output:" << std::endl;
        std::cout << libyaml_final_output.back() << std::endl;
        std::cout << "-----------------" << std::endl;
    }
    
    return libyaml_final_output;
}

// ---------------------------------------------------------------------------------
// ------------------------------ yaml-cpp test code -------------------------------
// ---------------------------------------------------------------------------------

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
        std::cout << err.what() << std::endl;
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
            std::cout << temp_result_holder << std::endl;
            yamlcpp_final_output += temp_result_holder;

            if(temp_result_holder.empty() && error_message_container->empty())
            {
                *error_message_container = "ERROR";
            }
        }
    }
    catch (const std::exception& err)
    {
        std::cout << err.what() << std::endl;
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

// ---------------------------------------------------------------------------------
// ---------------------------------- testcode -------------------------------------
// ---------------------------------------------------------------------------------

bool compareStringsCustom
    (const std::string compareMeOne, const std::string compareMeTwo, std::string& buffer)
{
    std::string::const_iterator ptrOne = compareMeOne.begin();
    std::string::const_iterator ptrTwo = compareMeTwo.begin();

    while (*ptrOne == *ptrTwo && ((ptrOne != compareMeOne.end()) || (ptrTwo != compareMeTwo.end())))
    {
        if(*ptrOne == *ptrTwo)
        {
            buffer += *ptrTwo;
        }
        ptrOne++;
        ptrTwo++;
    }

    if (!((ptrOne == compareMeOne.end())  && (ptrTwo == compareMeTwo.end())))
    {
         buffer += "(X)";
        return false;
    }
    else
    {
        return true;
    }
}