#include "normalization_test_utils.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


bool compareBufferCustom(const std::string compare_me_one, 
    const std::string compare_me_two, std::string& buffer)
{
    std::string::const_iterator ptrOne = compare_me_one.begin();
    std::string::const_iterator ptrTwo = compare_me_two.begin();

    while (*ptrOne == *ptrTwo && ((ptrOne != compare_me_one.end()) || (ptrTwo != compare_me_two.end())))
    {
        if(*ptrOne == *ptrTwo)
        {
            buffer += *ptrTwo;
        }
        ptrOne++;
        ptrTwo++;
    }

    if (!((ptrOne == compare_me_one.end())  && (ptrTwo == compare_me_two.end())))
    {
        buffer += "(X)";
        return false;
    }
    else
    {
        return true;
    }
}

void PrintNodes(std::vector<YAML::Node>* print_me)
{
    for (std::vector<YAML::Node>::iterator iterator = print_me->begin(); 
        iterator != print_me->end(); iterator++)
    {
        std::stringstream stream_node;

        iterator->SetStyle(YAML::EmitterStyle::Block);

        std::cout << *iterator << std::endl;
    }
}