#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace differential_parser
{
class ParserOutput
{
    public:
        virtual ~ParserOutput(){};
        virtual bool equivalent(ParserOutput* compared_object) = 0;
        virtual void* getData() = 0;
        virtual std::string* getError() = 0;
};

class Parser
{
    public:
        virtual std::string getName() = 0;
        virtual void* parse(const uint8_t* input, size_t input_size, std::string* error_code) = 0;
        virtual differential_parser::ParserOutput* normalize(void* input, std::string* error_code) = 0;
};
} // namespace differential_parser


#endif