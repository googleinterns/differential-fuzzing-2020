#include <string.h>
#include <assert.h>

#include "parser.h"

#include <iostream>

namespace differential_fuzzer
{
    struct AssociatedParserName
    {
        std::string data;
        AssociatedParserName* next_container;
    };

    struct EquivalenceParserOutputs
    {
        EquivalenceParserOutputs* next;
        void* representative;
        AssociatedParserName* container_basket;
    };

    // ---------------------------------------------------------------------------------
    // ----------------------------------- Deleters ------------------------------------
    // ---------------------------------------------------------------------------------

    void DeleteContainers(AssociatedParserName* delete_me)
    {    
        AssociatedParserName* prev = delete_me;

        while (delete_me != NULL)
        {
            
            delete_me = delete_me->next_container;

            // Makes deletions
            delete prev;
            
            // Move next

            prev = delete_me;
            
        }
        
    }

    void DeleteNodes (EquivalenceParserOutputs* delete_me)
    {
        EquivalenceParserOutputs* prev = delete_me;
        
        while (delete_me != NULL)
        {
            delete_me = delete_me->next;

            // Makes deletions
            delete((std::string *)(prev->representative));//(placeholder)
            DeleteContainers(prev->container_basket);
            delete prev;

            //Move next
            prev = delete_me;

        }
    }

    // ---------------------------------------------------------------------------------
    // --------------------------- struc print functions -------------------------------
    // ---------------------------------------------------------------------------------

    void PrintParserContainers(AssociatedParserName* head)
    {
        AssociatedParserName* ptr = head;

        if (ptr!=NULL)
        {
            std::cerr << "- ";
        }

        while (ptr!=NULL)
        {
            std::cerr << ptr->data;
            ptr = ptr->next_container;

            if (ptr==NULL)
            {
                std::cerr << std::endl;
            }
            else
            {
                std::cerr << std::endl << "- ";
            }
        }
    }


    void PrintNodes(EquivalenceParserOutputs* head)
    {
        EquivalenceParserOutputs* ptr = head;

        std::cerr << "--- Printing Results" << std::endl;

        int set_id = 1;

        while (ptr!=NULL)
        {
            std::cerr << "Set #" << set_id << ":" << std::endl;
            
            set_id++;

            if (ptr->container_basket!=NULL)
            {
                PrintParserContainers(ptr->container_basket);
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

    void AddDataToBasket(AssociatedParserName** head, std::string name)
    {
        AssociatedParserName* ptr =  new AssociatedParserName;

        ptr->data = name;
        ptr->next_container = *head;

        *head = ptr;
    }

    bool CheckAndAdd(differential_parser::Parser* parser, EquivalenceParserOutputs** head)
    {
        // Iterator:
        EquivalenceParserOutputs* ptr = *head;

        void* parser_output = parser->normalize(NULL);

        while (ptr != NULL)
        {
            if ( parser->equivalent(ptr->representative, parser_output))
            {
                // add to parse basket

                AddDataToBasket(&ptr->container_basket, parser->getName());
                delete((std::string *)(parser_output));     // (placeholder)
                return true;
            }
            ptr = ptr->next;

        }
        ptr = new EquivalenceParserOutputs;
        ptr->representative = parser_output;
        ptr->container_basket = new AssociatedParserName;
        ptr->container_basket->data = parser->getName();
        ptr->container_basket->next_container = NULL;
        ptr->next = *head;

        *head = ptr;
        // add to parse basket

        return false;
    }

    // ---------------------------------------------------------------------------------
    // ----------------------------- Fuzzing Methd -------------------------------------
    // ---------------------------------------------------------------------------------

    void DifferentiallyFuzz(differential_parser::Parser** parser_array, int number_of_parsers)
    {
        // Creates iteration structure
        EquivalenceParserOutputs *head = new EquivalenceParserOutputs;
        
        // For error reporting
        bool are_all_parsers_equal = true;

        // First case:
        head->representative = parser_array[0]->normalize(NULL);
        head->next = NULL;
        head->container_basket = new AssociatedParserName;
        head->container_basket->data = parser_array[0]->getName();
        head->container_basket->next_container = NULL;

        for (int i=1; i<number_of_parsers;i++)
        {
            are_all_parsers_equal = CheckAndAdd(parser_array[i], &head);
        }
        DeleteNodes(head);
    }
}


