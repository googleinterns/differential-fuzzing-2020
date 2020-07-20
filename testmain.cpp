#include <iostream>
#include <string>
#include <fstream>

// From git file
#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

// #include "differentialfuzzer.h"

struct thing
{
    char* t1;
    char* t2;
    char* t3;
};

// Main used for the purpose of minor testing
int main()
{
    std::cout << "Do thing!" << std::endl;
    
    std::cout << "----------- libyaml tests -----------" << std::endl;

    yaml_parser_t parser;
    yaml_event_t event;
    yaml_emitter_t emitter;

    memset(&parser, 0, sizeof(parser));
    memset(&event, 0, sizeof(event));
    memset(&emitter, 0, sizeof(emitter));
    int done = 0;

    // global-tag.yaml(yaml-cpp error)  mapping.yaml mappingother.yaml  strings.yaml
    std::string path_to_test_file = "examples/strings.yaml";

    FILE* output = fopen(path_to_test_file.c_str(),"r");
    FILE* output_for_emitter = fopen("tempyamloutput.txt","w");
    // FILE* output = fopen("tempyamloutput.txt","r");

    // size_t string_size = 17;
    // char start[17] = "\xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0";

    // size_t string_size = 6;
    // char start[6] = "\xf7\xbf\xbf\xbf!";

    // size_t string_size = 33;
    // char start[33] = "\x80|\x81|\x82|\x83|\x84|\x85|\x86|\x87|\x88|\x89|\x8a|\x8b|\x8c|\x8d|\x8e|\x8f|";

    // size_t string_size = 33;
    // char start[33] = "\xc0\xaf|\xe0\x80\xaf|\xf0\x80\x80\xaf|\xf8\x80\x80\x80\xaf|\xfc\x80\x80\x80\x80\xaf!";

    // size_t string_size = 2;
    // char start[2] = "o";

    // size_t string_size = 8;
    // char start[8] = "1e ]e \t";
    // const std::string temp = "folded: >\n    This entire block of text will be the value of 'folded', but this\n    time, all newlines will be replaced with a single space";
    // const std::string temp = "base: &base\n name: Everyone has same name\n\n foo: &foo\n <<: *base\n age: 10";

    // size_t string_size = 140;
    // char start[140] = "folded: >\n    This entire block of text will be the value of 'folded', but this\n    time, all newlines will be replaced with a single space";

    // size_t string_size = 74;
    // char start[74] = "base: &base\n name: Everyone has same name\n\n foo: &foo\n <<: *base\n age: 10";
    
    size_t string_size = 135;
    // char start[135] = "characterType :\n type1 :\n  attribute1 : something\n  attribute2 : something\n type2 :\n  attribute1 : something\n  attribute2 : something\n";

    string_size = 1000;
    // const std::string temp = std::string(start, string_size-1);

    // size_t string_size = strlen(start);
    
    // size_t string_size = 300;

    unsigned char* buffer = new unsigned char[1000];

    size_t written = 0;

    if(!yaml_parser_initialize(&parser))
    {
        return 0;
    }
    
    if(!yaml_emitter_initialize(&emitter))
    {
        return 0;
    }
    
    // yaml_parser_set_input_string(&parser, (yaml_char_t*)start, string_size - 1);
    yaml_parser_set_input_file(&parser,output);
    
    // yaml_emitter_set_output_file(&emitter,output_for_emitter); //output_for_emitter
    // yaml_emitter_set_output_file(&emitter,stdout); //output_for_emitter
    yaml_emitter_set_output_string(&emitter, buffer, string_size + 2, &written);

    yaml_emitter_set_unicode(&emitter, 1);

    // yaml_emitter_set_canonical(&emitter, 1);

    while (!done)
    {

        if (!yaml_parser_parse(&parser, &event)) 
        {
            std::cout << "----BAD PARSER----" << std::endl;

            break;
        }


        done = (event.type == YAML_STREAM_END_EVENT);
        // yaml_event_delete(&event);

        if(!yaml_emitter_emit(&emitter, &event))
        {
            std::cout << "----BAD EMITTER----" << std::endl;

            switch (emitter.error)
            {
                case YAML_MEMORY_ERROR:
                    fprintf(stderr, "Memory error: Not enough memory for emitting\n");
                    break;

                case YAML_WRITER_ERROR:
                    fprintf(stderr, "Writer error: %s\n", emitter.problem);
                    break;

                case YAML_EMITTER_ERROR:
                    fprintf(stderr, "Emitter error: %s\n", emitter.problem);
                    break;

                default:
                    /* Couldn't happen. */
                    fprintf(stderr, "Internal error\n");
                    break;
            }
            std::cout << std::string((char*)event.data.scalar.value) << std::endl;
        }
        
        // std::cout << "- Output from emitter: "<< buffer << std::endl;
    }

    std::cout << "- Print again not knowing size " << std::endl;

    // std::cout << buffer << std::endl;
    int pos_in_buffer = 0;

    int temp_interator_counter = string_size;

    while(buffer[pos_in_buffer]!=NULL)
    {
        if(buffer[pos_in_buffer] == '\n')
        {
            std::cout << "(x)";
        }
        std::cout << buffer[pos_in_buffer];

        pos_in_buffer++;
        temp_interator_counter--;
    }
    std::cout <<  "----" << std::endl;

    yaml_event_delete(&event); // may be leaking

    

    // yaml_parser_delete(&parser);
    fclose(output);

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;
    const uint8_t input_data[] = {'b', 'e'};
    int input_data_size = 2;
    const std::string hello((const char*)input_data, input_data_size);

    std::string yamlcpp_final_output;
    try
    {   
        YAML::Node node = YAML::LoadFile(path_to_test_file);
        // YAML::Node node = YAML::Load("[2, 3]");
        // YAML::Node node = YAML::Load(temp);
        // YAML::Node node = YAML::Load("Hello World");
        std::cout << "Node type: " << node.Type() << std::endl;
        // YAML::NodeType::Null;
        // YAML::NodeType::Scalar
        // YAML::NodeType::Sequence
        switch (node.Type())
        {
            case (YAML::NodeType::Null):
                yamlcpp_final_output = "- Null case\n";

                break;
            case (YAML::NodeType::Scalar):
                yamlcpp_final_output = node.Scalar();

                break;
            case (YAML::NodeType::Sequence):
                std::cout << "Sequence" << std::endl;
                for (std::size_t i=0;i<node.size();i++) 
                {
                    std::cout << "loop" << std::endl;
                    yamlcpp_final_output += "- " + std::string(node[i].as<char>(),1) + "\n";
                }
                break;
            case (YAML::NodeType::Map):
                std::cout << "map" << std::endl;
                for (YAML::const_iterator it=node.begin();it!=node.end();++it) 
                {
                    yamlcpp_final_output += "- " + it->first.as<std::string>() + "\n";

                    yamlcpp_final_output += node[it->first.as<std::string>()].as<std::string>() + "\n";

                    // std::cout << "- "<< it->first.as<std::string>() << std::endl;

                    // std::cout << node[it->first.as<std::string>()].as<std::string>() << std::endl;
                }
                break;
            case (YAML::NodeType::Undefined):
                yamlcpp_final_output = "Undef\n";

                break;
            default:
                yamlcpp_final_output = "ERROR: Unknown Input Type \n";
        }

        std::cout << "Output:" << std::endl;
        std::cout << yamlcpp_final_output << std::endl;
        std::cout << "--------:" << std::endl;

        // Makes selection of how to parse node
        // if (node.Type() == 1) // NULL
        // {
        //     yamlcpp_final_output = "- Null case";
        // }
        // else if (node.Type() == 2) // Scalar
        // {
        //     yamlcpp_final_output = node.Scalar();
        // }
        // else if (node.Type() == 3) // Sequence
        // {
        //     for (std::size_t i=0;i<node.size();i++) 
        //     {
        //         yamlcpp_final_output += "- " + node[i].as<std::string>() +"\n";
        //     }
        // }
        // else if (node.Type() == 4) // Dictionary
        // {
        //     std::cout << "----- Before looping: " << std::endl;

        //     for (YAML::const_iterator it=node.begin();it!=node.end();++it) 
        //     {
        //         std::cout << "- "<< it->first.as<std::string>() << std::endl;

        //         std::cout << node[it->first.as<std::string>()].as<std::string>() << std::endl;
        //     }
        //     std::cout << "----- After looping" << std::endl;
        // }
        // else if (node.Type() == 5) // Map (only for special cases, so it may never be called)
        // {
        //     std::cout << "I am a map!" << std::endl;
        // }
        // else // Unknown
        // {
        //     std::cerr << "ERROR: Unknown Input Type" << std::endl;
        // }
    }
    catch(const std::exception& err)
    {
        std::cout << err.what() << std::endl;
    }

    // yaml_parser_delete(&parser);
    // yaml_emitter_delete(&emitter);

    return 0;
}

// \0 in the middle