#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <openssl/bn.h>
#include <gcrypt.h>

#include "xmlparser.h"
#include "yamlparser.h"

#include <iostream>

void DifferentiallyFuzz(parser* parser_array[2], int number_of_parsers)
{
    bool are_all_parsers_equal = false;
    std::cout << parser_array[0]->normalize(NULL) << std::endl;
    std::cout << parser_array[1]->normalize(NULL) << std::endl;
    assert(parser_array[0]->normalize(NULL)==parser_array[1]->normalize(NULL));
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) 
{
    xmlparser xmlcase;
    yamlparser yamlcase;
    parser* array_of_parsers[2] = {dynamic_cast<parser*>(&xmlcase), dynamic_cast<parser*>(&yamlcase)};
    DifferentiallyFuzz(array_of_parsers, 2);
    return 0;
}

