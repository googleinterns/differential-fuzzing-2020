#include <iostream>
#include <string>
#include <assert.h>

#include "differential_fuzzer.h"


// ---------------------------------------------------------------------------------
// ----------------------------------- Deleters ------------------------------------
// ---------------------------------------------------------------------------------

void differential_fuzzer::parser::DeleteAssociatedParserName(AssociatedParserName* delete_me)
{
    while (delete_me != nullptr)
    {
        AssociatedParserName* next = delete_me->next_parser;

        delete delete_me;

        delete_me = next;
    }
}

void differential_fuzzer::parser::DeleteEquivalenceNormalizedOutputs
    (EquivalenceNormalizedOutputs* delete_me)
{
    while (delete_me != nullptr)
    {
        EquivalenceNormalizedOutputs* next = delete_me->next;

        // Makes deletions
        delete delete_me->representative;
        DeleteAssociatedParserName(delete_me->container_basket);
        delete delete_me;

        //Move next
        delete_me = next;
    }
}

// ---------------------------------------------------------------------------------
// --------------------------- struc print functions -------------------------------
// ---------------------------------------------------------------------------------

void differential_fuzzer::parser::PrintParserList(AssociatedParserName* head)
{
    AssociatedParserName* ptr = head;

    if (ptr != nullptr)
    {
        std::cerr << "- ";
    }

    while (ptr != nullptr)
    {
        std::cerr << ptr->name;

        ptr = ptr->next_parser;

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
void differential_fuzzer::parser::PrintEquivalenceNormalizedOutputs(EquivalenceNormalizedOutputs* head)
{
    EquivalenceNormalizedOutputs* ptr = head;

    std::cerr << "--- Printing Results" << std::endl;

    int set_id = 1;

    while (ptr != nullptr)
    {
        std::cerr << "Set #" << set_id << ":" << std::endl;

        set_id++;

        if (ptr->container_basket != nullptr)
        {
            PrintParserList(ptr->container_basket);
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
    ptr->next_parser = *head;

    *head = ptr;
}

// Function checks if a parser has an output that matches the representative of one 
// of the already existing EquivalenceNormalizedOutputs list. If the output of the new
// parser matches a representative, its name is saved to the AssociatedParserName 
// list of that particular EquivalenceNormalizedOutputs node. If the output of a new 
// parser does not match a previous representative, a new EquivalenceNormalizedOutputs 
// node is created, and added to the original EquivalenceNormalizedOutputs with the 
// output of the parser being saved as the representative of that 
// EquivalenceNormalizedOutputs node.
//
// If the parser output has matched a previous representative, the function returns
// true. If the parser output has not matched a previous representative, the
// function returns false
bool differential_fuzzer::parser::CheckAndAdd(differential_parser::Parser* parser, 
    EquivalenceNormalizedOutputs** head, const uint8_t* input_data, size_t size_of_input)
{
    // Iterator:
    EquivalenceNormalizedOutputs* ptr = *head;
    
    std::string* local_error = new std::string();

    void* temp_parse_holder = parser->parse(input_data, size_of_input, local_error);

    differential_parser::NormalizedOutput* parser_output = 
        parser->normalize(temp_parse_holder, local_error);

    // Iterate through the different EquivalenceNormalizedOutputs
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
    
    bool return_me = (*head == nullptr);

    ptr = new EquivalenceNormalizedOutputs;
    ptr->representative = parser_output;
    ptr->container_basket = new AssociatedParserName;
    ptr->container_basket->name = parser->getName();
    ptr->container_basket->next_parser = nullptr;
    ptr->next = *head;

    *head = ptr;

    return return_me;
}

bool differential_fuzzer::fuzzer::DifferentiallyFuzz(differential_parser::Parser** parser_array, 
    int number_of_parsers, const uint8_t* input_data, size_t size_of_input)
{
    if (number_of_parsers <= 0)
    {
        std::cerr << "ERROR: No parsers passed to 'DifferentiallyFuzz' function" 
            << std::endl;
        return false;
    }

    // Creates iteration structure
    parser::EquivalenceNormalizedOutputs* head = nullptr;

    bool all_parser_are_similar = true;

    for (int i = 0; i < number_of_parsers; i++)
    {
        if (!CheckAndAdd(parser_array[i], &head, input_data, size_of_input))
        {
            all_parser_are_similar = false;
        }
    }

    PrintEquivalenceNormalizedOutputs(head);
    DeleteEquivalenceNormalizedOutputs(head);

    return all_parser_are_similar;
}

