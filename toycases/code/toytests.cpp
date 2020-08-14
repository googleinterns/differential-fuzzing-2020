#include <iostream>
#include <string>

#include "base_parser.h"
#include "differential_fuzzer.h"
#include "toy_fuzz_int_parser.h"
#include "toy_fuzz_switch_parser.h"
#include "toy_fuzz_string_output_base.h"

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
// --- toy-Int:
// Output =(int)(*(const char*)input) + 100
// Error = empty
// --- toy-8:
// Output = input + " wears a hot "
// Nomalize = Output + "pair of glasses"
// Error = empty
// --- toy-Switch
// - First char = '0'
// Output = input + " wears a hot "
// Nomalize = Output + "pair of glasses"
// Error = empty
// - First char != '0'
// Output = input + " wears a pink "
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
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case1), 
        (differential_parser::Parser*)(&toy_string_case1)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());

    size_t size = 3;
    
    std::cout << "------ TEST 1: Equal Data, No Error Case " << std::endl << "(1 and 1)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest2()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[0] = {};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 2: Empty Case" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 0, Data, size));
}

void runTest3()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[1] = {(differential_parser::Parser*)(&toy_string_case1)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 3: One Case" << std::endl << "(1)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 1, Data, size));
}

void runTest4()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case1), 
        (differential_parser::Parser*)(&toy_string_case2)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 4: Different Data, No Error Case" << std::endl << "(1 and 2)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest5()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case3(" wears a hot hat", "", "toy-3", "");


    differential_parser::Parser* array_of_parsers[3] = {(differential_parser::Parser*)(&toy_string_case1), 
        (differential_parser::Parser*)(&toy_string_case2), (differential_parser::Parser*)(&toy_string_case3)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 5: Mixed Data, No Errors " << std::endl <<"(1 != 2, 3; (2 == 3))" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 3, Data, size));
}

void runTest6()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case4(" wears a hot hat", "", "toy-4", "ERROR: warm hat");


    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case2),
        (differential_parser::Parser*)(&toy_string_case4)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 6: Same Data, One Error " << std::endl <<"(2 == 4; 4 has error)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest7()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case4(" wears a hot hat", "", "toy-4", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case5(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case5),
        (differential_parser::Parser*)(&toy_string_case4)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 7: Different Data, Same Errors " << std::endl <<"(4 != 5)" << std::endl;
    
    assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest8()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case5(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case6(" wears a floatty hat", "", "toy-6", "ERROR: hat too hot");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case5),
        (differential_parser::Parser*)(&toy_string_case6)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 8: Same Data, Different Errors " << std::endl <<"(5 != 6; err same)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest9()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_int_differential_parser::ToyIntParser toy_int_case1;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case1),
        (differential_parser::Parser*)(&toy_int_case1)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 9: Different type conversion" << std::endl <<"(1 ?? 7)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest10()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_int_differential_parser::ToyIntParser toy_int_case1;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_int_case1),
        (differential_parser::Parser*)(&toy_string_case1)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 10: Different type conversion" << std::endl <<"(7 ?? 1)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest11()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case8),
        (differential_parser::Parser*)(&toy_string_case1)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 11: Bad Normalizer, No Error" << std::endl <<"(8 != 1)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTest12()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");
    toy_switch_differential_parser::ToySwitchParser toy_switch_case1;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_switch_case1),
        (differential_parser::Parser*)(&toy_string_case8)};
    
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

void runTest13()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case2),
        (differential_parser::Parser*)(&toy_string_case8)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST 13: Different Due to Normalizer" << std::endl <<"(2 != 8)" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

void runTestAll1()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case3(" wears a hot hat", "", "toy-3", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case4(" wears a hot hat", "", "toy-4", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case5(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case6(" wears a floatty hat", "", "toy-6", "ERROR: hat too hot");
    toy_int_differential_parser::ToyIntParser toy_int_case1;
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");
    toy_switch_differential_parser::ToySwitchParser toy_switch_case1;


    differential_parser::Parser* array_of_parsers[9] = {(differential_parser::Parser*)(&toy_string_case1),
        (differential_parser::Parser*)(&toy_string_case2), (differential_parser::Parser*)(&toy_string_case3), 
        (differential_parser::Parser*)(&toy_string_case4), (differential_parser::Parser*)(&toy_string_case5), 
        (differential_parser::Parser*)(&toy_string_case6), (differential_parser::Parser*)(&toy_int_case1),
        (differential_parser::Parser*)(&toy_string_case8), (differential_parser::Parser*)(&toy_switch_case1)};
    
    const uint8_t *Data = (uint8_t*)(std::string("bob").c_str());
    size_t size = 3;
    
    std::cout << "------ TEST All: Throw Everything at Differential Fuzzer" << std::endl <<"All of Cases" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 9, Data, size));
}

void runTestAll2()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case3(" wears a hot hat", "", "toy-3", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case4(" wears a hot hat", "", "toy-4", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case5(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case6(" wears a floatty hat", "", "toy-6", "ERROR: hat too hot");
    toy_int_differential_parser::ToyIntParser toy_int_case1;
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");
    toy_switch_differential_parser::ToySwitchParser toy_switch_case1;

    differential_parser::Parser* array_of_parsers[9] = {(differential_parser::Parser*)(&toy_string_case1),
        (differential_parser::Parser*)(&toy_string_case2), (differential_parser::Parser*)(&toy_string_case3), 
        (differential_parser::Parser*)(&toy_string_case4), (differential_parser::Parser*)(&toy_string_case5), 
        (differential_parser::Parser*)(&toy_string_case6), (differential_parser::Parser*)(&toy_int_case1),
        (differential_parser::Parser*)(&toy_string_case8), (differential_parser::Parser*)(&toy_switch_case1)};
    
    int integer_holder = int(30);
    const uint8_t *Data = (uint8_t*)(&integer_holder);
    size_t size = 3;
    
    std::cout << "------ TEST All: Throw Everything at Differential Fuzzer" << std::endl <<"All of Cases" << std::endl;
    
    assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 9, Data, size));
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
    runTest13();

    runTestAll1();
    runTestAll2();

    return 0;
}