#include <iostream>
#include <string>

#include "baseparser.h"
#include "differentialfuzzer.h"
#include "toyfuzz7parser.h"
#include "toyfuzz9parser.h"
#include "toyfuzzstringoutputbase.h"

// Key to toy case:
// --- toy-1:
// Output = input + " wears a cool hat"
// Error = empty
// --- toy-2:
// Output = input + " wears a hot "
// Nomalize = Output + "hat"
// Error = empty
// --- toy-3:
// Output = input + " wears a hot hat"
// Error = empty
// --- toy-4:
// Output = input + " wears a hot hat"
// Error = "ERROR: warm hat"
// --- toy-5:
// Output = input + " wears a floatty hat"
// Error = "ERROR: warm hat"
// --- toy-6:
// Output = input + " wears a floatty hat"
// Error = "ERROR: hat too hot"
// --- toy-7:
// Output =(int)(*(const char*)input) + 100
// Error = empty
// --- toy-8:
// Output = input + " wears a hot "
// Nomalize = Output + "pair of glasses"
// Error = empty

// ---------------------------------------------------------------------------------
// ------------------------------- Helper Methods ----------------------------------
// ---------------------------------------------------------------------------------

bool assess(bool result)
{
    std::cout << "------ Result: ";
    
    bool returnMe;

    if (result)
    {
        std::cout << "PASS" << std::endl;
        returnMe = true;
    }
    else
    {
        std::cout << "FAIL" << std::endl;
        returnMe = false;
    }    

    std::cout << std::endl;

    return returnMe;
}

// ---------------------------------------------------------------------------------
// ------------------------------------ Tests --------------------------------------
// ---------------------------------------------------------------------------------

void runTest1()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy1_case), 
        (differential_parser::Parser*)(&toy1_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());

    size_t size = 3;
    
    std::cout << "------ TEST 1: Equal Data, No Error Case " << std::endl << "(1 and 1)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest2()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[0] = {};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 2: Empty Case" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 0, Data, size));
}

void runTest3()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[1] = {(differential_parser::Parser*)(&toy1_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 3: One Case" << std::endl << "(1)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 1, Data, size));
}

void runTest4()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy2_case(" wears a hot ", "hat", "toy-2", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy1_case), 
        (differential_parser::Parser*)(&toy2_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 4: Different Data, No Error Case" << std::endl << "(1 and 2)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest5()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy2_case(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy3_case(" wears a hot hat", "", "toy-3", "");


    differential_parser::Parser* array_of_parsers[3] = {(differential_parser::Parser*)(&toy1_case), 
        (differential_parser::Parser*)(&toy2_case), (differential_parser::Parser*)(&toy3_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 5: Mixed Data, No Errors " << std::endl <<"(1 != 2, 3; (2 == 3))" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 3, Data, size));
}

void runTest6()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy2_case(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy4_case(" wears a hot hat", "", "toy-4", "ERROR: warm hat");


    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy2_case),
        (differential_parser::Parser*)(&toy4_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 6: Same Data, One Error " << std::endl <<"(2 == 4; 4 has error)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest7()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy4_case(" wears a hot hat", "", "toy-4", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy5_case(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy5_case),
        (differential_parser::Parser*)(&toy4_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 7: Different Data, Same Errors " << std::endl <<"(4 != 5)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest8()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy5_case(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy6_case(" wears a floatty hat", "", "toy-6", "ERROR: hat too hot");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy5_case),
        (differential_parser::Parser*)(&toy6_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 8: Same Data, Different Errors " << std::endl <<"(5 != 6; err same)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest9()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");
    toy7_differential_parser::Toy7Parser toy7_case;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy1_case),
        (differential_parser::Parser*)(&toy7_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 9: Different type conversion" << std::endl <<"(1 ?? 7)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest10()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");
    toy7_differential_parser::Toy7Parser toy7_case;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy7_case),
        (differential_parser::Parser*)(&toy1_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 10: Different type conversion" << std::endl <<"(7 ?? 1)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest11()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy8_case(" wears a hot ", "pair of glasses", "toy-8", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy8_case),
        (differential_parser::Parser*)(&toy1_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 11: Bad Normalizer, No Error" << std::endl <<"(8 != 1)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest12()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy8_case(" wears a hot ", "pair of glasses", "toy-8", "");
    toy9_differential_parser::Toy9Parser toy9_case;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy9_case),
        (differential_parser::Parser*)(&toy8_case)};
    
    const uint8_t *DataOne = (uint8_t*)(std::string("1bob").c_str());
    size_t size = 4;
    
    std::cout << "------ TEST 12: Tests Uncertain Case" << std::endl << "8 vs 9" << std::endl;
    
    std::cout << "--- Local Test 1:" << std::endl;

    bool finalAssessment = assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, DataOne, size));

    const uint8_t *DataZero = (uint8_t*)(std::string("0bob").c_str());

    std::cout << "--- Local Test 2:" << std::endl;

    finalAssessment = finalAssessment && assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, DataZero, size));

    std::cout << "--- Conclusion:" << std::endl;
    assess(finalAssessment);
}

void runTestAll1()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy2_case(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy3_case(" wears a hot hat", "", "toy-3", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy4_case(" wears a hot hat", "", "toy-4", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy5_case(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy6_case(" wears a floatty hat", "", "toy-6", "ERROR: hat too hot");
    toy7_differential_parser::Toy7Parser toy7_case;
    toy_generic_string_helper::ToyFuzzGenericStringParser toy8_case(" wears a hot ", "pair of glasses", "toy-8", "");

    differential_parser::Parser* array_of_parsers[8] = {(differential_parser::Parser*)(&toy1_case),
        (differential_parser::Parser*)(&toy2_case), (differential_parser::Parser*)(&toy3_case), 
        (differential_parser::Parser*)(&toy4_case), (differential_parser::Parser*)(&toy5_case), 
        (differential_parser::Parser*)(&toy6_case), (differential_parser::Parser*)(&toy7_case),
        (differential_parser::Parser*)(&toy8_case)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST All: Throw Everything at Differential Fuzzer" << std::endl <<"All of Cases" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 8, Data, size));
}

void runTestAll2()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy1_case(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy2_case(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy3_case(" wears a hot hat", "", "toy-3", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy4_case(" wears a hot hat", "", "toy-4", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy5_case(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy6_case(" wears a floatty hat", "", "toy-6", "ERROR: hat too hot");
    toy7_differential_parser::Toy7Parser toy7_case;
    toy_generic_string_helper::ToyFuzzGenericStringParser toy8_case(" wears a hot ", "pair of glasses", "toy-8", "");

    differential_parser::Parser* array_of_parsers[8] = {(differential_parser::Parser*)(&toy1_case),
        (differential_parser::Parser*)(&toy2_case), (differential_parser::Parser*)(&toy3_case), 
        (differential_parser::Parser*)(&toy4_case), (differential_parser::Parser*)(&toy5_case), 
        (differential_parser::Parser*)(&toy6_case), (differential_parser::Parser*)(&toy7_case),
        (differential_parser::Parser*)(&toy8_case)};
    
    int integer_holder = int(30);
    const uint8_t *Data = (uint8_t*)(&integer_holder);
    size_t size = 3;
    
    std::cout << "------ TEST All: Throw Everything at Differential Fuzzer" << std::endl <<"All of Cases" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 8, Data, size));
}

// ---------------------------------------------------------------------------------
// ----------------------------------- Executable ----------------------------------
// ---------------------------------------------------------------------------------

int main()
{
    runTest1();
    runTest2();
    runTest3();
    runTest4();
    runTest5();
    runTest6();
    runTest7();
    runTest8();
    runTest9();
    runTest10();
    runTest11();
    runTest12();

    runTestAll1();
    runTestAll2();

    return 0;
}