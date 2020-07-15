#include <string.h>
#include <assert.h>

#include "differentialfuzzer.h"


#include <iostream>

// ---------------------------------------------------------------------------------
// ----------------------------------- Deleters ------------------------------------
// ---------------------------------------------------------------------------------

void differential_fuzzer::parser::DeleteAssociatedParserName(AssociatedParserName* delete_me)
{    
    AssociatedParserName* prev = delete_me;

    while (delete_me != nullptr)
    {
        delete_me = delete_me->next_container;

        delete prev;
        
        prev = delete_me;
        
    }
    
}

void differential_fuzzer::parser::DeleteEquivalenceParserOutputs(EquivalenceParserOutputs* delete_me)
{
    EquivalenceParserOutputs* prev = delete_me;
    
    while (delete_me != nullptr)
    {
        delete_me = delete_me->next;

        // Makes deletions
        delete prev->representative;
        DeleteAssociatedParserName(prev->container_basket);
        delete prev;

        //Move next
        prev = delete_me;

    }
}

// ---------------------------------------------------------------------------------
// --------------------------- struc print functions -------------------------------
// ---------------------------------------------------------------------------------

void differential_fuzzer::parser::PrintAssociatedParserName(AssociatedParserName* head)
{
    AssociatedParserName* ptr = head;

    if (ptr != nullptr)
    {
        std::cerr << "- ";
    }

    while (ptr != nullptr)
    {
        std::cerr << ptr->name;

        ptr = ptr->next_container;

        if (ptr == nullptr)
        {
            std::cerr << std::endl;
        }
        else
        {
            std::cerr << std::endl << "- ";
        }
    }
}

// Ouput in cerr:
// Ex.:
//--- Printing Results
// Set #1:
// - parser_2
// Set #2:
// - parser_1
// - parser_3
void differential_fuzzer::parser::PrintEquivalenceParserOutputs(EquivalenceParserOutputs* head)
{
    EquivalenceParserOutputs* ptr = head;

    std::cerr << "--- Printing Results" << std::endl;

    int set_id = 1;

    while (ptr != nullptr)
    {
        std::cerr << "Set #" << set_id << ":" << std::endl;
        
        set_id++;

        if (ptr->container_basket != nullptr)
        {
            PrintAssociatedParserName(ptr->container_basket);
        }
        else
        {
            std::cerr << "ERROR: basket not selected" << std::endl;
        }
        
        ptr = ptr->next;

    }
}

// ---------------------------------------------------------------------------------
// ------------------------------ struc functions ----------------------------------
// ---------------------------------------------------------------------------------


void differential_fuzzer::parser::AddToAssociatedParserName(AssociatedParserName** head, 
    std::string name)
{
    AssociatedParserName* ptr =  new AssociatedParserName;

    ptr->name = name;
    ptr->next_container = *head;

    *head = ptr;
}

// Function checks if a parser has an output that matches the representative of one 
// of the already existing EquivalenceParserOutputs list. If the output of the new
// parser matches a representative, its name is saved to the AssociatedParserName 
// list of that particular EquivalenceParserOutputs node. If the output of a new 
// parser does not match a previous representative, a new EquivalenceParserOutputs 
// node is created, and added to the original EquivalenceParserOutputs with the 
// output of the parser being saved as the representative of that 
// EquivalenceParserOutputs node.
//
// If the parser output has matched a previous representative, the function returns
// true. If the parser output has not matched a previous representative, the
// function returns false
bool differential_fuzzer::parser::CheckAndAdd(differential_parser::Parser* parser, 
    EquivalenceParserOutputs** head, const uint8_t* input_data, size_t size_of_input)
{
    // Iterator:
    EquivalenceParserOutputs* ptr = *head;

    std::string* local_error = new std::string();;

    void* temp_parse_holder = parser->parse(input_data, size_of_input, local_error);

    differential_parser::ParserOutput* parser_output = 
        parser->normalize(temp_parse_holder, local_error);

    // Iterate through the different EquivalenceParserOutputs
    while (ptr != nullptr)
    {
        if (parser_output->equivalent(ptr->representative))
        {
            AddToAssociatedParserName(&ptr->container_basket, parser->getName());

            delete parser_output;     

            return true;
        }
        ptr = ptr->next;

    }
    ptr = new EquivalenceParserOutputs;
    ptr->representative = parser_output;
    ptr->container_basket = new AssociatedParserName;
    ptr->container_basket->name = parser->getName();
    ptr->container_basket->next_container = nullptr;
    ptr->next = *head;

    *head = ptr;
    // add to parse basket

    return false;
}

void differential_fuzzer::fuzzer::DifferentiallyFuzz(differential_parser::Parser** parser_array, 
    int number_of_parsers, const uint8_t* input_data, size_t size_of_input)
{
    if (number_of_parsers <= 0)
    {
        std::cerr << "ERROR: No parsers passed to 'DifferentiallyFuzz' function" 
            << std::endl;
        return;
    }

    // Creates iteration structure
    parser::EquivalenceParserOutputs* head = new parser::EquivalenceParserOutputs;
    
    // For error reporting
    bool are_all_parsers_equal = true;


    // First case:
    std::string* local_error  = new std::string();
    void* temp_parse_holder = parser_array[0]->parse(input_data, size_of_input, local_error);

    head->representative = parser_array[0]->normalize(temp_parse_holder, local_error);
    head->next = nullptr;
    head->container_basket = new parser::AssociatedParserName;
    head->container_basket->name = parser_array[0]->getName();
    head->container_basket->next_container = nullptr;

    for (int i = 1; i < number_of_parsers; i++)
    {
        are_all_parsers_equal = CheckAndAdd(parser_array[i], &head, input_data, size_of_input);
    }

    DeleteEquivalenceParserOutputs(head);
}

