#include "xmlparser.h"
#include <string>

#include <iostream>

namespace xml_differential_parser
{
// ---------------------------------------------------------------------------------
// ------------------------------ XmlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

XmlParserOutput::XmlParserOutput(std::string* info)
{
    this->data = info;
}

XmlParserOutput::~XmlParserOutput()
{
    if (this->data != nullptr)
    {
        delete this->data;
    }
}

bool XmlParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* XmlParserOutput::getData()
{
    return static_cast<void*> (this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string XmlParser::getName()
{
    return "xml";
}

void* XmlParser::parse(uint8_t* input, size_t input_size)
{
    return nullptr;
}

differential_parser::ParserOutput* XmlParser::normalize
    (void* input)
{
    std::string* output = new std::string("xml!");

    differential_parser::ParserOutput* returnMe = new
        XmlParserOutput (output);
    
    return returnMe;
}
} // namespace xml_differential_parser
