#include "libyamlparser.h"
#include "include/yaml.h"

#include <iostream>

namespace libyaml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ LibyamlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

LibyamlParserOutput::LibyamlParserOutput(std::string* info)
{
    this->data = info;
}

LibyamlParserOutput::~LibyamlParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool LibyamlParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* LibyamlParserOutput::getData()
{
    return static_cast<void*>(this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- LibyamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string LibyamlParser::getName()
{
    return "yaml";
}


void* LibyamlParser::parse(const uint8_t* input, size_t input_size)
{
    yaml_parser_t parser;
    yaml_event_t event;
    yaml_emitter_t emitter;

    unsigned char buffer[input_size];

    bool done = false;

    if(!yaml_parser_initialize(&parser))
    {
        return 0;
    }

    yaml_parser_set_input_string(&parser, (yaml_char_t*)input, input_size /*- 1*/);

    if(!yaml_emitter_initialize(&emitter))
    {
        return 0;
    }

    size_t written = 0;

    yaml_emitter_set_output_string(&emitter, buffer, input_size /*-1*/, &written);

    yaml_emitter_set_unicode(&emitter, 1);

    while (!done)
    {

        if (!yaml_parser_parse(&parser, &event)) 
        {
            break;
        }

        done = (event.type == YAML_STREAM_END_EVENT);
        
        yaml_emitter_emit(&emitter, &event);
    }

    yaml_event_delete(&event); // may be leaking
    yaml_parser_delete(&parser);
    yaml_emitter_delete(&emitter);

    std::string* return_me = new std::string((char*)buffer, input_size);

    return (void*)return_me;
}

differential_parser::ParserOutput* LibyamlParser::normalize
    (void* input)
{   
    //std::string((char*)input)
    differential_parser::ParserOutput* return_me = new
        LibyamlParserOutput((std::string*)input);
    return return_me;
}
}