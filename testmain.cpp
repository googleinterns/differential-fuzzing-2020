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

   

    char start[17] = "\xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0";

    size_t string_size = strlen(start);
    
    unsigned char buffer[string_size];
    size_t written = 0;

    char data[4] = "bob";
    char* otherdata = data;

    if(!yaml_parser_initialize(&parser) || !yaml_emitter_initialize(&emitter))
    return 0;



    yaml_parser_set_input_string(&parser, (yaml_char_t*)start, string_size - 1);
    yaml_emitter_set_output_string(&emitter, buffer, string_size - 1, &written);

    yaml_emitter_set_unicode(&emitter, 1);

    std::cout << "- Oh no test: "<< parser.raw_buffer.start << std::endl;

    while (!done)
    {
        std::cout << "Loop! " << data << std::endl;

        if (!yaml_parser_parse(&parser, &event)) 
        {
            std::cout << "Nothing happens!" << std::endl;

            break;
        }

        if(!yaml_emitter_emit(&emitter, &event))
        {
            std::cout << "bad emitter" << std::endl;

        }

        std::cout << "- Output from emitter: "<< buffer << std::endl;

        std::cout << "- Output from raw buffer parser: "<< (char*)parser.raw_buffer.start << std::endl;

        done = (event.type == YAML_STREAM_END_EVENT);
        // yaml_event_delete(&event);
    }

    // yaml_parser_delete(&parser);

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;

    YAML::Node primes = YAML::Load("[\xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0\xb0\xb0, \xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0]");
    
    for (YAML::const_iterator it=primes.begin();it!=primes.end();++it) 
    {
        std::string yaml_cpp_loop_temp = it->as<std::string>();
        std::cout << yaml_cpp_loop_temp << std::endl;
        
        std::cout << "libyaml == yaml-cpp: " << 
            (strcmp((char*)parser.raw_buffer.start, yaml_cpp_loop_temp.c_str()) == 0) << std::endl;
        std::cout << "---" << std::endl;

        // parser.raw_buffer.start==yaml_cpp_loop_temp
    }

    yaml_parser_delete(&parser);
    yaml_emitter_delete(&emitter);
    yaml_event_delete(&event); // may be leaking

    return 0;
}