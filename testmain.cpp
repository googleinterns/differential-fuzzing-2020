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


    
    
    int failed = 0;
    int k;

    // FILE* fil = fopen("testfile.txt","r");
    // std::string* result = new std::string();
    
    // yaml_parser_t parser;
    // yaml_token_t token; 

    // yaml_parser_initialize(&parser);
    
    // // yaml_event_t  event; 
    // // unsigned const char testcase[17] = "\xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0";
    // // std::cout << testcase << "\n";

    // // yaml_parser_set_input_string(&parser, testcase, 17 /*17*/);

    // bool done = false;

    // int counter = 0;

    // yaml_parser_scan(&parser, &token);

    // // while(!done)
    // // {
    // //     std::cout << "-In loop"<< "\n";
    // //     if(!yaml_parser_scan(&parser, &token))
    // //     {
    // //         std::cout << "OOPS"<< "\n";

    // //     }
    // //     std::cout << "-Scan done"<< "\n";
    // //     done = (token.type==YAML_STREAM_END_TOKEN);
    // //     counter++;
        
    // //     std::cout << "-End loop"<< "\n";
    // // }
    // yaml_token_delete(&token);
    
    // yaml_parser_delete(&parser);
    
    // fclose(fil);

    FILE *fh = fopen("strings.yaml","r");
    yaml_parser_t parser;
    yaml_token_t token; 
    thing* thinging = new thing;

    if (!yaml_parser_initialize(&parser))
        fputs("Failed to initialize parser!\n", stderr);
    if (fh == NULL)
        fputs("Failed to open file!\n", stderr);

    yaml_parser_set_input_file(&parser, fh);

    char** datap;
    char* tk;

    yaml_parser_scan(&parser, &token);

    tk = (char*)token.data.scalar.value;    // ERROR

    std::cout << "Result of my thing: "<< tk << std::endl;
    
    yaml_parser_delete(&parser);

    fclose(fh);


    std::cout << "----------- yaml-cpp tests -----------" << std::endl;

    YAML::Node primes = YAML::Load("[\xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0, \xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0]");
    
    for (YAML::const_iterator it=primes.begin();it!=primes.end();++it) 
    {
        std::cout << it->as<std::string>() << "\n";
    }

    

    return 0;
}