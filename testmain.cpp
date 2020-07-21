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

void printBuffer(char* buffer)
{
    std::cout << "- Print again not knowing size " << std::endl;

    int pos_in_buffer = 0;

    while(buffer[pos_in_buffer]!=NULL)
    {
        std::cout << buffer[pos_in_buffer];

        pos_in_buffer++;
    }
}

// Main used for the purpose of minor testing
int main()
{
    std::cout << "Do thing!" << std::endl;
    
    std::cout << "----------- libyaml tests -----------" << std::endl;

    yaml_parser_t parser;
    memset(&parser, 0, sizeof(parser));

    yaml_document_t document;

    yaml_document_t document_list[3];
    memset(document_list, 0, 3*sizeof(yaml_document_t));
    // memset(&document, 0, sizeof(document));

    // yaml_token_t token;
    // memset(&token, 0, sizeof(token));

    yaml_emitter_t emitter;
    memset(&emitter, 0, sizeof(emitter));

    // yaml_event_t event;
    // memset(&event, 0, sizeof(event));
    
    int done = 0;

    // global-tag.yaml(yaml-cpp error)  mapping.yaml mappingother.yaml  strings.yaml
    std::string path_to_test_file = "examples/strings.yaml";

    FILE* output = fopen(path_to_test_file.c_str(),"r");
    FILE* output_for_emitter = fopen("tempyamloutput.txt","w");

    // size_t string_size = 74;
    // char start[74] = "base: &base\n name: Everyone has same name\n\n foo: &foo\n <<: *base\n age: 10";
    
    // size_t string_size = 135;
    // char start[135] = "characterType :\n type1 :\n  attribute1 : something\n  attribute2 : something\n type2 :\n  attribute1 : something\n  attribute2 : something\n";

    size_t string_size = 1000;
    // const std::string temp = std::string(start, string_size-1);

    unsigned char* buffer = new unsigned char[1000];

    size_t written = 0;

    if(!yaml_parser_initialize(&parser))
    {
        return 0;
    }
    
    yaml_emitter_initialize(&emitter);
    
    // yaml_parser_set_input_string(&parser, (yaml_char_t*)start, string_size - 1);
    yaml_parser_set_input_file(&parser,output);
    yaml_emitter_set_unicode(&emitter, 1);

    // yaml_emitter_set_output_file(&emitter,output_for_emitter); //output_for_emitter
    // yaml_emitter_set_output_file(&emitter,stdout); //output_for_emitter

    yaml_emitter_set_output_string(&emitter, buffer, string_size + 1, &written);
    yaml_emitter_open(&emitter);

    int count = 0;

    while (!done)
    {
        std::cout << "- Loop" << std::endl;
        
        if (!yaml_parser_load(&parser, &document)) 
        {
            std::cout << "----BAD PARSER----" << std::endl;

            break;
        }

        done = (!yaml_document_get_root_node(&document));

        yaml_document_delete(&document);

        if (!done)  
        {
            //add more stuff here
            count ++;
        }
        // tk = (char*)token.data.scalar.value;
        
        

        // yaml_event_delete(&event);

        // if(!yaml_emitter_emit(&emitter, &event))
        // {
        //     std::cout << "----BAD EMITTER----" << std::endl;

        //     switch (emitter.error)
        //     {
        //         case YAML_MEMORY_ERROR:
        //             fprintf(stderr, "Memory error: Not enough memory for emitting\n");
        //             break;

        //         case YAML_WRITER_ERROR:
        //             fprintf(stderr, "Writer error: %s\n", emitter.problem);
        //             break;

        //         case YAML_EMITTER_ERROR:
        //             fprintf(stderr, "Emitter error: %s\n", emitter.problem);
        //             break;

        //         default:
        //             /* Couldn't happen. */
        //             fprintf(stderr, "Internal error\n");
        //             break;
        //     }
        //     std::cout << std::string((char*)event.data.scalar.value) << std::endl;
        // }
        
        // std::cout << "- Output from emitter: "<< buffer << std::endl;
        
        // std::cout << (char*)token.data.scalar.value << std::endl;
    }

    yaml_parser_delete(&parser);
    fclose(output);
    yaml_emitter_close(&emitter);
    yaml_emitter_delete(&emitter);

    // std::cout << "Nodes: "<< count<< std::endl;
    // std::cout <<  "----" << std::endl;

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;
    const uint8_t input_data[] = {'b', 'e'};
    int input_data_size = 2;
    const std::string hello((const char*)input_data, input_data_size);

    std::string yamlcpp_final_output;
    try
    {   
        YAML::Node node = YAML::LoadFile(path_to_test_file);
        // YAML::Node node = YAML::Load(temp);
        std::cout << "Node type: " << node.Type() << std::endl;

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
    }
    catch(const std::exception& err)
    {
        std::cout << err.what() << std::endl;
    }
    return 0;
}

// \0 in the middle