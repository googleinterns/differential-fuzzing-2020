#include <iostream>
#include <string>

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
    int done = 0;

    FILE* output = fopen("strings.yaml","r");
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

    size_t string_size = 8;
    char start[8] = "1e ]e \t";

    // size_t string_size = strlen(start);
    
    // size_t string_size = 300;

    unsigned char* buffer = new unsigned char[100];

    size_t written = 0;

    if(!yaml_parser_initialize(&parser))
    {
        return 0;
    }

    yaml_parser_set_input_string(&parser, (yaml_char_t*)start, string_size - 1);
    // yaml_parser_set_input_file(&parser,output);
    
    if(!yaml_emitter_initialize(&emitter))
    {
        return 0;
    }

    
    // yaml_emitter_set_output_file(&emitter,output_for_emitter);
    yaml_emitter_set_output_string(&emitter, buffer, string_size-1, &written);

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

        }

        // std::cout << "- Output from emitter: "<< buffer << std::endl;

        std::cout << "- Output from raw buffer parser: "<< (char*)parser.raw_buffer.start << std::endl;
        std::cout << "- Output from emitter: "<< buffer << std::endl;
    }

    std::cout << "- Print again not knowing size "<< buffer << std::endl;

    int pos_in_buffer = 0;

    int temp_interator_counter = string_size;

    while(buffer[pos_in_buffer]!=NULL && temp_interator_counter>1)
    {
        std::cout << buffer[pos_in_buffer];
        pos_in_buffer++;
        temp_interator_counter--;
    }
    std::cout << std::endl;

    yaml_event_delete(&event); // may be leaking

    

    // yaml_parser_delete(&parser);
    fclose(output);

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;
    const uint8_t input_data[] = {'b', 'e'};
    int input_data_size = 2;
    const std::string hello((const char*)input_data, input_data_size);
    try
    {   
        YAML::Node primes = YAML::LoadFile("strings.yaml");
        // YAML::Node primes = YAML::Load("["+ std::string(start) +"]");
        // YAML::Node primes = YAML::Load("["+ hello +"]");
        std::cout << primes["literal-block"] << std::endl;

        // for (YAML::const_iterator it=primes.begin();it!=primes.end();++it) 
        // {
        //     std::string yaml_cpp_loop_temp = it->as<std::string>();
            
        //     std::cout << yaml_cpp_loop_temp << std::endl;
        //     std::cout << "libyaml == yaml-cpp: " << 
        //         (strcmp((const char*)yaml_cpp_loop_temp.c_str(),(char*)(buffer)) == 0) << std::endl;
        //     std::cout << "---" << std::endl;

        //     // parser.raw_buffer.start==yaml_cpp_loop_temp
        // }
    }
    catch(const std::exception& err)
    {
        std::cout << err.what() << std::endl;
    }

    // YAML::Emitter out;
    // out << std::string(start);
    // std::cout << out.c_str() << std::endl;

    // yaml_parser_delete(&parser);
    // yaml_emitter_delete(&emitter);

    return 0;
}

// \0 in the middle