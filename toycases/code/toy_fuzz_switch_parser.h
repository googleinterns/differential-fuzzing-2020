#ifndef TOY_FUZZ_SWITCH_PARSER_H
#define TOY_FUZZ_SWITCH_PARSER_H

#include "base_parser.h"

#include <string>

namespace toy_switch_differential_parser
{
// ---------------------------------------------------------------------------------
// ---------------------------------- ToySwitchParser -----------------------------------
// ---------------------------------------------------------------------------------

class ToySwitchParser : virtual public differential_parser::Parser
{
    
    public:
        std::string getName();
        void* parse(const uint8_t* input, size_t input_size, std::string* error_code);
        differential_parser::ParserOutput* normalize(void* input, std::string* error_code);
};
} // namespace yaml_differential_parser


#endif