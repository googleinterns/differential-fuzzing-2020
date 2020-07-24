#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <assert.h>
#include <fstream>

// From git file
#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

std::string parse_escaped(yaml_char_t * str, size_t length)
{
    int i;
    char c;

    std::string escaped_parsed_final;

    for (i = 0; i < length; i++) 
    {
        c = *(str + i);
        if (c == '\\')
            escaped_parsed_final += ("\\\\");
        else if (c == '\0')
            escaped_parsed_final += ("\\0");
        else if (c == '\b')
            escaped_parsed_final += ("\\b");
        else if (c == '\n')
            escaped_parsed_final += ("\\n");
        else if (c == '\r')
            escaped_parsed_final += ("\\r");
        else if (c == '\t')
            escaped_parsed_final += ("\\t");
        else
        escaped_parsed_final += c;
    }
    return escaped_parsed_final;
}

int usage(int ret) 
{
    fprintf(stderr, "Usage: libyaml-parser [--flow (on|off|keep)] [<input-file>]\n");
    return ret;
}

std::string doYamlExample(std::string name_of_file)
{
    FILE *input;
    yaml_parser_t parser;
    yaml_event_t event;
    
    int flow = -1; 
    int i = 0;
    int foundfile = 0;

    std::string libyaml_final_output = "";

    input = fopen(name_of_file.c_str(), "rb");
    foundfile = 1;

    if (!foundfile) 
    {
        input = stdin;
    }
    assert(input);

    if (!yaml_parser_initialize(&parser)) 
    {
        return "ERROR: Could not initialize the parser object\n";
    }
    yaml_parser_set_input_file(&parser, input);

    while (1) 
    {
        yaml_event_type_t type;
        if (!yaml_parser_parse(&parser, &event)) 
        {
            if ( parser.problem_mark.line || parser.problem_mark.column ) 
            {
                fprintf(stderr, "Parse error: %s\nLine: %lu Column: %lu\n",
                    parser.problem,
                    (unsigned long)parser.problem_mark.line + 1,
                    (unsigned long)parser.problem_mark.column + 1);
            }
            else {
                fprintf(stderr, "Parse error: %s\n", parser.problem);
            }
            return "ERROR: Bad parsing";
        }
        type = event.type;

        if (type == YAML_NO_EVENT)
            libyaml_final_output += ("???\n");
        else if (type == YAML_STREAM_START_EVENT)
            libyaml_final_output += ("+STR\n");
        else if (type == YAML_STREAM_END_EVENT)
            libyaml_final_output += ("-STR\n");
        else if (type == YAML_DOCUMENT_START_EVENT) 
        {
            libyaml_final_output += ("+DOC");
            if (!event.data.document_start.implicit)
                libyaml_final_output += (" ---");
            libyaml_final_output += ("\n");
        }
        else if (type == YAML_DOCUMENT_END_EVENT) 
        {
            libyaml_final_output += ("-DOC");
            if (!event.data.document_end.implicit)
                libyaml_final_output += (" ...");
            libyaml_final_output += ("\n");
        }
        else if (type == YAML_MAPPING_START_EVENT) 
        {
            libyaml_final_output += ("+MAP");
            if (flow == 0 && event.data.mapping_start.style == YAML_FLOW_MAPPING_STYLE)
                libyaml_final_output += (" {}");
            else if (flow == 1)
                libyaml_final_output += (" {}");
            if (event.data.mapping_start.anchor)
            {
                std::string temp_translator = ((char*)event.data.mapping_start.anchor);
                libyaml_final_output += " &" + temp_translator + "";
            }       
            if (event.data.mapping_start.tag)
            {
                std::string temp_translator = ((char*)event.data.mapping_start.tag);
                libyaml_final_output += " <&" + temp_translator + ">";
            }                
            libyaml_final_output += ("\n");
        }
        else if (type == YAML_MAPPING_END_EVENT)
            libyaml_final_output += ("-MAP\n");
        else if (type == YAML_SEQUENCE_START_EVENT) 
        {
            libyaml_final_output += ("+SEQ");
            if (flow == 0 && event.data.sequence_start.style == YAML_FLOW_SEQUENCE_STYLE)
                libyaml_final_output += (" []");
            else if (flow == 1)
                libyaml_final_output += (" []");
            if (event.data.sequence_start.anchor)
            if (event.data.scalar.anchor)
            {
                std::string temp_translator = ((char*)event.data.sequence_start.anchor);
                libyaml_final_output += " &" + temp_translator;
            }
            if (event.data.sequence_start.tag) 
            {
                std::string temp_translator = ((char*)event.data.sequence_start.tag);
                libyaml_final_output += (" <"+ temp_translator + ">");
            }
            libyaml_final_output += ("\n");
        }
        else if (type == YAML_SEQUENCE_END_EVENT)
            libyaml_final_output += ("-SEQ\n");
        else if (type == YAML_SCALAR_EVENT) 
        {
            libyaml_final_output += ("=VAL");
            if (event.data.scalar.anchor)
            {
                std::string temp_translator = ((char*)event.data.scalar.anchor);
                libyaml_final_output += " &" + temp_translator;
            }
            if (event.data.scalar.tag)
            {
                std::string temp_translator = ((char*)event.data.scalar.tag);
                libyaml_final_output += " <"+temp_translator + ">";
            }
            switch (event.data.scalar.style) 
            {
            case YAML_PLAIN_SCALAR_STYLE:
                libyaml_final_output += (" :");
                break;
            case YAML_SINGLE_QUOTED_SCALAR_STYLE:
                libyaml_final_output += (" '");
                break;
            case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
                libyaml_final_output += (" \"");
                break;
            case YAML_LITERAL_SCALAR_STYLE:
                libyaml_final_output += (" |");
                break;
            case YAML_FOLDED_SCALAR_STYLE:
                libyaml_final_output += (" >");
                break;
            case YAML_ANY_SCALAR_STYLE:
                abort();
            }
            libyaml_final_output += parse_escaped(event.data.scalar.value, event.data.scalar.length);
            libyaml_final_output += ("\n");
        }
        else if (type == YAML_ALIAS_EVENT)
        {
            std::string temp_translator = ((char*) event.data.alias.anchor);
            libyaml_final_output += "=ALI "+temp_translator + "\n";
        }
        else
            abort();

        yaml_event_delete(&event);

        if (type == YAML_STREAM_END_EVENT)
            break;
    }

    assert(!fclose(input));
    yaml_parser_delete(&parser);
    fflush(stdout);

    return libyaml_final_output;
}

// ---------------------------------------------------------------------------------
// ------------------------------ yaml-cpp test code -------------------------------
// ---------------------------------------------------------------------------------

std::string parseYamlCppNode(YAML::Node& head)
{
    std::stack <YAML::Node> iteration_list_stack;

    iteration_list_stack.push(head);

    std::string yamlcpp_final_output = "";



    while (!iteration_list_stack.empty())
    {
        YAML::Node base_iterator = iteration_list_stack.top();
        iteration_list_stack.pop();

        switch(base_iterator.Type())
        {
            case YAML::NodeType::Null:
            {
                yamlcpp_final_output += "- Null case";
                break;
            }
            case YAML::NodeType::Scalar:
            {
                yamlcpp_final_output += "- " + base_iterator.as<std::string>() + "\n";
                break;
            }
            case YAML::NodeType::Sequence:
            {
                for (int i = base_iterator.size() - 1; i >= 0; i--) 
                {
                    std::cout << base_iterator[i].Tag() << std::endl;
                    iteration_list_stack.push(base_iterator[i]);
                }                
                break;
            }
            case YAML::NodeType::Map:
            {
                std::stack <YAML::const_iterator> loca_iterators_temp_stack;

                for (YAML::const_iterator it = base_iterator.begin(); it != base_iterator.end(); ++it) 
                {
                    loca_iterators_temp_stack.push(it);
                }

                while (!loca_iterators_temp_stack.empty())
                {
                    YAML::const_iterator it = loca_iterators_temp_stack.top();
                    loca_iterators_temp_stack.pop();

                    iteration_list_stack.push(it->second);
                    iteration_list_stack.push(it->first);
                }                
                break;
            }
            case YAML::NodeType::Undefined:
            {
                yamlcpp_final_output += "- Undef \n";
                break;
            }
            default:
            {
                yamlcpp_final_output += "- ERROR: Unknown Input Type \n";
            }
        }
    }
    return yamlcpp_final_output;
}

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main(int argc, char* args[])
{
    std::cout << "----------- libyaml tests -----------" << std::endl;

    // doYamlExample(args[1]);

    std::string libyaml_final_output = doYamlExample(args[1]);
    std::cout << libyaml_final_output << std::endl;

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;

    std::string yamlcpp_final_output;
    try
    {   
        YAML::Node node = YAML::LoadFile(args[1]);
        // YAML::Node node = YAML::Load("[1, 2, 3]");
        std::cout << "Node type: " << node.Type() << std::endl;

        yamlcpp_final_output = parseYamlCppNode(node);
    }
    catch (const std::exception& err)
    {
        std::cout << err.what() << std::endl;
    }

    std::cout << "--------yaml-cpp Output:" << std::endl;
    std::cout << yamlcpp_final_output << std::endl;
    std::cout << "--------" << std::endl;

    return 0;
}

// \0 in the middle