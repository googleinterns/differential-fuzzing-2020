#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include <string>

namespace differential_parser
{
class NormalizedOutput
{
    public:
        virtual ~NormalizedOutput() {};
        virtual bool equivalent(NormalizedOutput* compared_object) = 0;
        virtual void* getData() = 0;
        virtual std::string* getError() = 0;
};

class Parser
{
    public:
        virtual std::string getName() = 0;
        virtual void* parse(const uint8_t* input, size_t input_size, std::string* error_code) = 0;
        virtual differential_parser::NormalizedOutput* normalize(void* input, std::string* error_code) = 0;
};
} // namespace differential_parser


#endif