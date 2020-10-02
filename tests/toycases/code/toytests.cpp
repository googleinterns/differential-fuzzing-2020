// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

bool Assess(bool result)
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

// Compares a fuzz target case against itself
void RunSelfAgainstSelfTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case1), 
        (differential_parser::Parser*)(&toy_string_case1)};

    const uint8_t *Data = (uint8_t*) (("Bob"));

    size_t size = 3;

    std::cout << "------ TEST 1: Equal Data, No Error Case " << std::endl << "(1 and 1)" << std::endl;

    Assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Give differential fuzzer case with no Parser given
void RunEmptyTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[0] = {};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 2: Empty Case" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 0, Data, size));
}

// Give a single case to test
void RunOneParserTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");

    differential_parser::Parser* array_of_parsers[1] = {(differential_parser::Parser*)(&toy_string_case1)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 3: One Case" << std::endl << "(1)" << std::endl;

    Assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 1, Data, size));
}

// Give two parsers that act exactly the same
void RunTwoEqualParsersTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case1), 
        (differential_parser::Parser*)(&toy_string_case2)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 4: Different Data, No Error Case" << std::endl << "(1 and 2)" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Test with two parsers that are equal, and one that is different
void RunDiverseDifferentParsersTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case3(" wears a hot hat", "", "toy-3", "");


    differential_parser::Parser* array_of_parsers[3] = {(differential_parser::Parser*)(&toy_string_case1), 
        (differential_parser::Parser*)(&toy_string_case2), (differential_parser::Parser*)(&toy_string_case3)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 5: Mixed Data, No Errors " << std::endl <<"(1 != 2, 3; (2 == 3))" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 3, Data, size));
}

// Test with parsers that will have the same result, but will different due to error
void RunDifferentDueToErrorTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case4(" wears a hot hat", "", "toy-4", "ERROR: warm hat");


    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case2),
        (differential_parser::Parser*)(&toy_string_case4)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 6: Same Data, One Error " << std::endl <<"(2 == 4; 4 has error)" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Test where parsers have different data and errors
void RunOneErrorTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case4(" wears a hot hat", "", "toy-4", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case5(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case5),
        (differential_parser::Parser*)(&toy_string_case4)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 7: Different Data, Same Errors " << std::endl <<"(4 != 5)" << std::endl;

    Assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Test where parsers have the same results, but different errors
void RunDifferentErrorsTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case5(" wears a floatty ", "hat", "toy-5", "ERROR: warm hat");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case6(" wears a floatty hat", "", "toy-6", "ERROR: hat too hot");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case5),
        (differential_parser::Parser*)(&toy_string_case6)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 8: Same Data, Different Errors " << std::endl <<"(5 != 6; err same)" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Checks for parsers that use different internal data types to parse information
// looking at comparing string to integer
void RunDifferentTypesTestCase1()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_int_differential_parser::ToyIntParser toy_int_case1;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case1),
        (differential_parser::Parser*)(&toy_int_case1)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 9: Different type conversion" << std::endl <<"(1 ?? 7)" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Checks for parsers that use different internal data types to parse information
// looking at comparing integer to string
void RunDifferentTypesTestCase2()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_int_differential_parser::ToyIntParser toy_int_case1;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_int_case1),
        (differential_parser::Parser*)(&toy_string_case1)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 10: Different type conversion" << std::endl <<"(7 ?? 1)" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Test for cases that have different parsers and normalizaers
void RunDifferentCasesWithDifferentNormalizersTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case1(" wears a cool hat", "", "toy-1", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case8),
        (differential_parser::Parser*)(&toy_string_case1)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 11: Bad Normalizer, No Error" << std::endl <<"(8 != 1)" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Test where we look at a parser that changes how it parses information based
// on the input
void RunInputDependentParserTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");
    toy_switch_differential_parser::ToySwitchParser toy_switch_case1;

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_switch_case1),
        (differential_parser::Parser*)(&toy_string_case8)};

    const uint8_t *DataOne = (uint8_t*) ("1bob");
    size_t size = 4;

    std::cout << "------ TEST 12: Tests Uncertain Case" << std::endl << "8 vs 9" << std::endl;

    std::cout << "--- Local Test 1:" << std::endl;

    bool finalAssessment = Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, DataOne, size));

    const uint8_t *DataZero = (uint8_t*)("0bob");

    std::cout << "--- Local Test 2:" << std::endl;

    finalAssessment = finalAssessment && Assess(differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, DataZero, size));

    std::cout << "--- Conclusion:" << std::endl;
    Assess(finalAssessment);
}

// Test results when two parsers are different due to the normalizer
void RunDifferenceByNormalizerTestCase()
{
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case2(" wears a hot ", "hat", "toy-2", "");
    toy_generic_string_helper::ToyFuzzGenericStringParser toy_string_case8(" wears a hot ", "pair of glasses", "toy-8", "");

    differential_parser::Parser* array_of_parsers[2] = {(differential_parser::Parser*)(&toy_string_case2),
        (differential_parser::Parser*)(&toy_string_case8)};

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST 13: Different Due to Normalizer" << std::endl <<"(2 != 8)" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 2, Data, size));
}

// Run test with all cases with a string based input
void RunTestAll1()
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

    const uint8_t *Data = (uint8_t*) (("Bob"));
    size_t size = 3;

    std::cout << "------ TEST All: Throw Everything at Differential Fuzzer" << std::endl <<"All of Cases" << std::endl;

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 9, Data, size));
}

// Run test with all cases with an int based input
void RunTestAll2()
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

    Assess(!differential_fuzzer::fuzzer::DifferentiallyFuzz(array_of_parsers, 9, Data, size));
}

// ---------------------------------------------------------------------------------
// ----------------------------------- Executable ----------------------------------
// ---------------------------------------------------------------------------------

int main()
{
    RunSelfAgainstSelfTestCase();
    RunEmptyTestCase();
    RunOneParserTestCase();
    RunTwoEqualParsersTestCase();
    RunDiverseDifferentParsersTestCase();
    RunDifferentDueToErrorTestCase();
    RunOneErrorTestCase();
    RunDifferentErrorsTestCase();
    RunDifferentTypesTestCase1();
    RunDifferentTypesTestCase2();
    RunDifferentCasesWithDifferentNormalizersTestCase();
    RunInputDependentParserTestCase();
    RunDifferenceByNormalizerTestCase();

    RunTestAll1();
    RunTestAll2();

    return 0;
}