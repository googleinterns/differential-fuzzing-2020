#include <iostream>
#include <string>

#include "baseparser.h"
#include "differentialfuzzer.h"
#include "toyfuzz1parser.h"
#include "toyfuzz2parser.h"
#include "toyfuzz3parser.h"
#include "toyfuzz4parser.h"

// ---------------------------------------------------------------------------------
// ------------------------------- Helper Methods ----------------------------------
// ---------------------------------------------------------------------------------

void assess(bool result)
{
    std::cout << "------ Result: ";
    if(result)
    {
        std::cout << "PASS" << std::endl;
    }
    else
    {
        std::cout << "FAIL" << std::endl;
    }    

    std::cout << std::endl;
}

// ---------------------------------------------------------------------------------
// ------------------------------------ Tests --------------------------------------
// ---------------------------------------------------------------------------------

void runTest1()
{
    toy1_differential_parser::Toy1Parser toyl_case;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toyl_case), 
        (differential_parser::Parser*)(&toyl_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 1: Equal Case (1 and 1)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest2()
{
    toy1_differential_parser::Toy1Parser toyl_case;

    differential_parser::Parser* array_of_parsers[0] = {};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 2: Empty Case (1)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 0, Data, size));
}

void runTest3()
{
    toy1_differential_parser::Toy1Parser toyl_case;

    differential_parser::Parser* array_of_parsers[1] = {(differential_parser::Parser*)(&toyl_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 3: One Case (1)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 1, Data, size));
}

// ---------------------------------------------------------------------------------
// ----------------------------------- Executable ----------------------------------
// ---------------------------------------------------------------------------------

int main()
{
    runTest1();
    runTest2();
    runTest3();
    return 0;
}