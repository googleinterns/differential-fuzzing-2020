#include "xmlparser.h"
#include <string>

#include <iostream>

// ---------------------------------------------------------------------------------
// ------------------------------ XmlParserOutput ---------------------------------
// ---------------------------------------------------------------------------------

xml_differential_parser::XmlParserOutput::XmlParserOutput(std::string* info)
{
    this->data = info;
}

xml_differential_parser::XmlParserOutput::~XmlParserOutput()
{
    delete this->data;
}

bool xml_differential_parser::XmlParserOutput::equivalent(ParserOutput* compared_object)
{
    return *(std::string*)this->getData() == *(std::string*)compared_object->getData();
}

void* xml_differential_parser::XmlParserOutput::getData()
{
    return static_cast<void*> (this->data);
}

// ---------------------------------------------------------------------------------
// ---------------------------------- YamlParser -----------------------------------
// ---------------------------------------------------------------------------------

std::string xml_differential_parser::XmlParser::getName()
{
    return "xml";
}

void* xml_differential_parser::XmlParser::parse(uint8_t* input, size_t input_size)
{
    return nullptr;
}

differential_parser::ParserOutput* xml_differential_parser::XmlParser::normalize
    (void* input)
{
    std::string* output = new std::string("xml!");

    differential_parser::ParserOutput* returnMe = new
        xml_differential_parser::XmlParserOutput (output);
    
    return returnMe;
}