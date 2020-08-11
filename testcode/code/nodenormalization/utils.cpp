#include "utils.h"

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

void addKeyToNode(YAML::Node* destinty, std::string key)
{
    (*destinty)[key];
}

bool positionAnalysis(char* add_to_me, char reference_character, bool map_mode)
{
    if (reference_character == 'M')
    {
        if (map_mode)
        {
            *add_to_me = 'K';
        }
        else
        {
            *add_to_me = 'V';
        }
        return !map_mode;
    }
    else if (reference_character == 'S')
    {
        *add_to_me = 'L';
    }
    else
    {
        *add_to_me = 'U';
    }

    return map_mode;
}

void addToMap(std::map<std::string, std::string>* anchor_map, 
    std::string* anchor, std::string* anchor_data)
{
    if (anchor != nullptr)
    {
        if (anchor_map->count(*anchor))
        {
            (*anchor_map)[*anchor] = *anchor_data;
        }
        else
        {
            anchor_map->insert({*anchor, *anchor_data});
        }
    }
}

bool addToMapDirective(std::map<std::string, std::string>* anchor_map, 
    std::stack<std::string>* anchor_save_stack, std::stack<std::string>* anchor_data_save_stack,
    int& subtract_count, bool interest_in_saving)
{
    if (!anchor_save_stack->empty() && interest_in_saving)
    {
        addToMap(anchor_map, &anchor_save_stack->top(), &anchor_data_save_stack->top());
        anchor_save_stack->pop();
        anchor_data_save_stack->pop();
        subtract_count = 2;
        return false;
    }

    return interest_in_saving;
}

bool addToStack(std::stack<std::string>* anchor_save_stack,int* subtract_count, char* anchor)
{
    anchor_save_stack->push(anchor);
    *subtract_count = 2;

    return true;
}


void addInfoToDataStack(std::stack<std::string>* anchor_data, 
    std::string info)
{
    if (!anchor_data->empty())
    {
        std::string& temp = anchor_data->top();
        temp += info;
    }
    else
    {
        anchor_data->push(info);
    }
}

std::string addTag(std::string* tag)
{
    std::string temp_translator = *tag;

    if(temp_translator == "?" || temp_translator == "!")
    {
        return "";
    }
    return temp_translator + " ";
}

void addToNode(YAML::Node* addToMe, YAML::Node* addMe, std::stack<YAML::Node>* key_stack, char* tracking_current_type)
{
    std::cout << *tracking_current_type << "----"<< std::endl;
    if (*tracking_current_type == 'L')
    {
        std::cout << *addToMe << "--L--" << *addMe << std::endl;
        addToMe->push_back(*addMe);
    }
    else if (*tracking_current_type == 'K')
    {
        std::cout << *addToMe << "--K--" << *addMe << std::endl;
        key_stack->push(*addMe);
        (*addToMe)[*addMe] = "";
    }
    else if (*tracking_current_type == 'V')
    {
        std::cout << *addToMe << "--V--" << *addMe << std::endl;
        (*addToMe)[key_stack->top()] = *addMe;
        key_stack->pop();
    }
}

std::string parseLibyaml(std::string name_of_file, std::string* error_message_container)
{
    return name_of_file;
}

std::vector<YAML::Node> normalizeLibyaml(std::string name_of_file, std::string* error_message_container)
{
    FILE *input;
    yaml_parser_t parser;
    yaml_event_t event;

    std::vector<YAML::Node> libyaml_final_output;
    // libyaml_final_output.push_back(YAML::Node());
    
    bool interest_in_saving = false;

    int subtract_count = 2;

    std::map<std::string, std::string> anchor_map;

    std::stack<std::string> anchor_save_stack;
    
    std::stack<std::string> anchor_data_save_stack;

    std::stack<YAML::Node> key_stack;


    std::stack<char> mode_stack;

    mode_stack.push(' ');

    std::stack<bool> map_mode_stack;

    bool map_mode = true;
    

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
        YAML::Node new_node;

        yaml_event_type_t type;

        char tracking_current_type;

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

                interest_in_saving = addToMapDirective(&anchor_map, &anchor_save_stack, 
                    &anchor_data_save_stack, subtract_count, interest_in_saving);

                break;
            case YAML_DOCUMENT_END_EVENT:

                // dump code

                break;            
            case YAML_DOCUMENT_START_EVENT:

                // dump code

                break;

            case YAML_MAPPING_END_EVENT:

                std::cout << "map end" << std::endl;

                mode_stack.pop();

                if(mode_stack.top()=='M')
                {
                    map_mode = map_mode_stack.top();
                    map_mode_stack.pop();
                }

                if (libyaml_final_output.size() > 1)
                {
                    char temp_position_info;

                    positionAnalysis(&temp_position_info, mode_stack.top(), !map_mode);

                    YAML::Node temp_node = libyaml_final_output.back();

                    libyaml_final_output.pop_back();
                    
                    addToNode(&libyaml_final_output.back(), &temp_node, &key_stack, &temp_position_info);
                }

                break;
            case YAML_SEQUENCE_END_EVENT:

                std::cout << "sequence end" << std::endl;

                mode_stack.pop();

                if (libyaml_final_output.size() > 1)
                {
                    char temp_position_info;

                    positionAnalysis(&temp_position_info, mode_stack.top(), map_mode);

                    YAML::Node temp_node = libyaml_final_output.back();

                    libyaml_final_output.pop_back();

                    addToNode(&libyaml_final_output.back(), &temp_node, &key_stack, &temp_position_info);
                }

                break;
            case YAML_MAPPING_START_EVENT:

                std::cout << "map start" << std::endl;
                
                libyaml_final_output.push_back(YAML::Node());

                if (!mode_stack.empty())
                {
                    positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);
                }

                if (mode_stack.top()=='M')
                {
                    map_mode_stack.push(!map_mode);
                }

                // if (event.data.mapping_start.anchor)
                // {
                //     //handle anchors
                // }      

                mode_stack.push('M');
                map_mode = true;

                if (event.data.mapping_start.tag)
                {
                    //more mapps cases
                }                

                break;
            case YAML_SEQUENCE_START_EVENT:

                std::cout << "new sequence" << std::endl;

                libyaml_final_output.push_back(YAML::Node());

                if (mode_stack.empty())
                {
                    std::cout << "Create" << std::endl;
                    libyaml_final_output.push_back(YAML::Node());
                }

                // if (event.data.sequence_start.anchor)
                // {
                //     //handle anchors
                // }      

                mode_stack.push('S');

                if (event.data.sequence_start.tag) 
                {
                    //more squences cases
                }

                break;

            case YAML_SCALAR_EVENT:
            {

                std::cout << "new scalar" << std::endl;

                map_mode = positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);
                
                YAML::Node addMe(std::string((char*)event.data.scalar.value, event.data.scalar.length));

                if(event.data.scalar.tag)
                {
                    std::string temp_tag_translator = ((char*)event.data.scalar.tag);

                    addMe.SetTag(temp_tag_translator);                    
                }
                else
                {
                    addMe.SetTag("?");
                }

                addToNode(&libyaml_final_output.back(), &addMe, &key_stack, &tracking_current_type);
                break;
            }
            case YAML_ALIAS_EVENT:
            {
                std::string temp_translator = ((char*) event.data.alias.anchor);

                if ((libyaml_final_output.back())[temp_translator])
                {
                    // UNTESTED
                    map_mode = positionAnalysis(&tracking_current_type, mode_stack.top(), map_mode);

                    YAML::Node temp = (libyaml_final_output.back())[temp_translator];

                    addToNode(&libyaml_final_output.back(), &temp, &key_stack, &tracking_current_type);
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
                break;
            }
            default: 

                break;
        }
        
        yaml_event_delete(&event);

        if (type == YAML_STREAM_END_EVENT)
            break;

        std::cout << "Size:" << libyaml_final_output.size() << std::endl;
    }

    assert(!fclose(input));

    yaml_parser_delete(&parser);

    fflush(stdout);
    
    std::cout << "I got to the end of the parser!" << std::endl;
    std::cout << "Size:" << libyaml_final_output.size() << std::endl;
    return libyaml_final_output;
}

// ---------------------------------------------------------------------------------
// ------------------------------ yaml-cpp test code -------------------------------
// ---------------------------------------------------------------------------------

std::vector<YAML::Node> parseYamlCpp(std::string parse_me, 
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

std::string normalizeYamlCpp(std::vector<YAML::Node>* nodes, 
                                std::string* error_message_container)
{
    std::string yamlcpp_final_output;

    if(nodes->empty())
    {
        *error_message_container = "ERROR";
        return yamlcpp_final_output;
    }

    try
    {   
        for (std::vector<YAML::Node>::iterator it = nodes->begin(); 
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


std::string normalizeYamlCppNode(YAML::Node* head, std::string* error_message_container)
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

        // if (tag_holder != "?" && tag_holder != "!")
        // {
        //     yamlcpp_final_output += tag_holder + " ";
        // }
        yamlcpp_final_output += tag_holder;


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

bool compareStringsCustom(std::string compareMeOne, std::string compareMeTwo, std::string& buffer)
{
    std::string::iterator ptrOne = compareMeOne.begin();
    std::string::iterator ptrTwo = compareMeTwo.begin();

    while (*ptrOne == *ptrTwo && ((ptrOne != compareMeOne.end()) || (ptrTwo != compareMeTwo.end())))
    {
        if(*ptrOne == *ptrTwo)
        {
            buffer += *ptrTwo;
        }
        ptrOne++;
        ptrTwo++;
    }

    if(!((ptrOne == compareMeOne.end())  && (ptrTwo == compareMeTwo.end())))
    {
        return false;
    }
    else
    {
        return true;
    }
}