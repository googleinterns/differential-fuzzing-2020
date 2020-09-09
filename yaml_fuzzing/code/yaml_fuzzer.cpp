#include "base_parser.h"
#include "differential_fuzzer.h"
#include "libyaml_parser.h"
#include "yamlcpp_parser.h"

#include <time.h>       /* time */

void StackCallTrick1(const uint8_t *Data, size_t size)
{
    bool thing_mcthing = false;
    assert(thing_mcthing);
}

void StackCallTrick2(const uint8_t *Data, size_t size)
{        
    bool hibbidy_hobbidy_here_is_my_hobby = false;
    assert(hibbidy_hobbidy_here_is_my_hobby);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t size) 
{
    differential_parser::Parser* array_of_parsers[2] = {
        (differential_parser::Parser*)(libyaml_differential_parser::LibyamlParser::GetStaticInstance()), 
        (differential_parser::Parser*)(yamlcpp_differential_parser::YamlCppParser ::GetStaticInstance())};

    bool fuzzers_are_equal = 
        differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size);

    assert(fuzzers_are_equal);

    // srand(time(NULL));
    
    // if(rand() % 100 < 50)
    // {
    //     StackCallTrick1(Data, size);
    // }
    // else
    // {
    //     StackCallTrick2(Data, size);
    // }
    return 0;
}

