#include <iostream>
#include <string>
#include <stack>
#include <assert.h>
#include <fstream>

// From git file
#include "include/yaml.h"
#include "yaml-cpp/yaml.h"

// ---------------------------------------------------------------------------------
// ------------------------------- libyaml test code -------------------------------
// ---------------------------------------------------------------------------------

void printBuffer(char* buffer)
{
    std::cout << "- Print again not knowing size " << std::endl;

    int pos_in_buffer = 0;

    while(buffer[pos_in_buffer]!=NULL)
    {
        std::cout << buffer[pos_in_buffer];

        pos_in_buffer++;
    }
}

void print_escaped(yaml_char_t * str, size_t length)
{
    int i;
    char c;

    for (i = 0; i < length; i++) {
        c = *(str + i);
        if (c == '\\')
            printf("\\\\");
        else if (c == '\0')
            printf("\\0");
        else if (c == '\b')
            printf("\\b");
        else if (c == '\n')
            printf("\\n");
        else if (c == '\r')
            printf("\\r");
        else if (c == '\t')
            printf("\\t");
        else
            printf("%c", c);
    }
}

int usage(int ret) {
    fprintf(stderr, "Usage: libyaml-parser [--flow (on|off|keep)] [<input-file>]\n");
    return ret;
}

int doYamlExample(std::string name_of_file)
{
    FILE *input;
    yaml_parser_t parser;
    yaml_event_t event;
    int flow = -1; /** default no flow style collections */
    int i = 0;
    int foundfile = 0;

    input = fopen(name_of_file.c_str(), "rb");
    foundfile = 1;

    if (!foundfile) {
        input = stdin;
    }
    assert(input);

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Could not initialize the parser object\n");
        return 1;
    }
    yaml_parser_set_input_file(&parser, input);

    while (1) {
        yaml_event_type_t type;
        if (!yaml_parser_parse(&parser, &event)) {
            if ( parser.problem_mark.line || parser.problem_mark.column ) {
                fprintf(stderr, "Parse error: %s\nLine: %lu Column: %lu\n",
                    parser.problem,
                    (unsigned long)parser.problem_mark.line + 1,
                    (unsigned long)parser.problem_mark.column + 1);
            }
            else {
                fprintf(stderr, "Parse error: %s\n", parser.problem);
            }
            return 1;
        }
        type = event.type;

        if (type == YAML_NO_EVENT)
            printf("???\n");
        else if (type == YAML_STREAM_START_EVENT)
            printf("+STR\n");
        else if (type == YAML_STREAM_END_EVENT)
            printf("-STR\n");
        else if (type == YAML_DOCUMENT_START_EVENT) {
            printf("+DOC");
            if (!event.data.document_start.implicit)
                printf(" ---");
            printf("\n");
        }
        else if (type == YAML_DOCUMENT_END_EVENT) {
            printf("-DOC");
            if (!event.data.document_end.implicit)
                printf(" ...");
            printf("\n");
        }
        else if (type == YAML_MAPPING_START_EVENT) {
            printf("+MAP");
            if (flow == 0 && event.data.mapping_start.style == YAML_FLOW_MAPPING_STYLE)
                printf(" {}");
            else if (flow == 1)
                printf(" {}");
            if (event.data.mapping_start.anchor)
                printf(" &%s", event.data.mapping_start.anchor);
            if (event.data.mapping_start.tag)
                printf(" <%s>", event.data.mapping_start.tag);
            printf("\n");
        }
        else if (type == YAML_MAPPING_END_EVENT)
            printf("-MAP\n");
        else if (type == YAML_SEQUENCE_START_EVENT) {
            printf("+SEQ");
            if (flow == 0 && event.data.sequence_start.style == YAML_FLOW_SEQUENCE_STYLE)
                printf(" []");
            else if (flow == 1)
                printf(" []");
            if (event.data.sequence_start.anchor)
                printf(" &%s", event.data.sequence_start.anchor);
            if (event.data.sequence_start.tag)
                printf(" <%s>", event.data.sequence_start.tag);
            printf("\n");
        }
        else if (type == YAML_SEQUENCE_END_EVENT)
            printf("-SEQ\n");
        else if (type == YAML_SCALAR_EVENT) {
            printf("=VAL");
            if (event.data.scalar.anchor)
                printf(" &%s", event.data.scalar.anchor);
            if (event.data.scalar.tag)
                printf(" <%s>", event.data.scalar.tag);
            switch (event.data.scalar.style) {
            case YAML_PLAIN_SCALAR_STYLE:
                printf(" :");
                break;
            case YAML_SINGLE_QUOTED_SCALAR_STYLE:
                printf(" '");
                break;
            case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
                printf(" \"");
                break;
            case YAML_LITERAL_SCALAR_STYLE:
                printf(" |");
                break;
            case YAML_FOLDED_SCALAR_STYLE:
                printf(" >");
                break;
            case YAML_ANY_SCALAR_STYLE:
                abort();
            }
            print_escaped(event.data.scalar.value, event.data.scalar.length);
            printf("\n");
        }
        else if (type == YAML_ALIAS_EVENT)
            printf("=ALI *%s\n", event.data.alias.anchor);
        else
            abort();

        yaml_event_delete(&event);

        if (type == YAML_STREAM_END_EVENT)
            break;
    }

    assert(!fclose(input));
    yaml_parser_delete(&parser);
    fflush(stdout);

    return 0;
}

// ---------------------------------------------------------------------------------
// ------------------------------ yaml-cpp test code -------------------------------
// ---------------------------------------------------------------------------------

std::string parseYamlCppNode(YAML::Node head)
{
    std::cout << "---- In parser" <<std::endl;
    std::stack <YAML::Node> iteration_list;
    iteration_list.push(head);

    std::string yamlcpp_final_output = "";

    while(!iteration_list.empty())
    {
        YAML::Node base_iterator = iteration_list.top();
        iteration_list.pop();

        std::cout << "- Switch time" <<std::endl;
        switch (base_iterator.Type())
        {
            case (YAML::NodeType::Null):
                yamlcpp_final_output += "- Null case";

                break;
            case (YAML::NodeType::Scalar):
                std::cout << "-scalar-" << std::endl;
                yamlcpp_final_output += "- " + base_iterator.as<std::string>() + "\n";
                std::cout << yamlcpp_final_output << std::endl;
                break;

            case (YAML::NodeType::Sequence):
                std::cout << "-seq-" << std::endl;
                for (int i = base_iterator.size() - 1; i >= 0; i--) 
                {
                    std::cout << i << std::endl;
                    iteration_list.push(base_iterator[i]);
                }
                break;
            case (YAML::NodeType::Map):
                std::cout << "-map-" << std::endl;
                iteration_list.push(base_iterator.begin()->second);
                iteration_list.push(base_iterator.begin()->first);
                break;
            case (YAML::NodeType::Undefined):
                break;
            default:
                yamlcpp_final_output += "ERROR: Unknown Input Type \n";
        }    
    }

    return yamlcpp_final_output;
}

// ---------------------------------------------------------------------------------
// -------------------------------------- main -------------------------------------
// ---------------------------------------------------------------------------------
int main()
{
    std::cout << "Do thing!" << std::endl;
    
    std::cout << "----------- libyaml tests -----------" << std::endl;
    
    // anchors.yaml(wrong_? 4)  array.yaml  global-tag.yaml(wrong_err 3)  json.yaml  
    // mapping.yaml  numbers.yaml
    // strings.yaml  tags.yaml  yaml-version.yaml
    // NEW: multimap.yaml multisequence.yaml
    std::string path_to_test_file = "examples/multisequence.yaml";

    doYamlExample(path_to_test_file);

    FILE* output = fopen(path_to_test_file.c_str(),"rb");

    std::cout << "----------- yaml-cpp tests -----------" << std::endl;

    std::string yamlcpp_final_output;
    try
    {   
        YAML::Node node = YAML::LoadFile(path_to_test_file);
        // YAML::Node node = YAML::Load("[1, 2, 3]");
        std::cout << "Node type: " << node.Type() << std::endl;


        switch (node.Type())
        {
            case (YAML::NodeType::Null):
                yamlcpp_final_output += "- Null case\n";

                break;
            case (YAML::NodeType::Scalar):
                yamlcpp_final_output += node.Scalar() + "\n";
                break;

            case (YAML::NodeType::Sequence):
                std::cout << "Sequence" << std::endl;
                for (std::size_t i=0;i<node.size();i++) 
                {
                    yamlcpp_final_output += parseYamlCppNode(node[i]);
                }
                break;

            case (YAML::NodeType::Map):
                std::cout << "map start" << std::endl;
                for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) 
                {
                    yamlcpp_final_output += parseYamlCppNode(it->first);
                    yamlcpp_final_output += parseYamlCppNode(it->second);
                }
                break;

            case (YAML::NodeType::Undefined):
                yamlcpp_final_output = "Undef\n";

                break;
            default:
                yamlcpp_final_output = "ERROR: Unknown Input Type \n";
        }    
        
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