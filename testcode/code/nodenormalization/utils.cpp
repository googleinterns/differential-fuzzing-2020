#include "utils.h"

bool compareStringsCustom
    (const std::string compare_me_one, const std::string compare_me_two, std::string& buffer)
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
