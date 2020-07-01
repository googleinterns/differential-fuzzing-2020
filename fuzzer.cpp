#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "xmlparser.h"
#include "yamlparser.h"
#include "parser.h"

#include <iostream>

struct ParserContainer
{
    std::string data;
    ParserContainer* next_container;
};

struct Node
{
    Node* next;
    void* representative;
    ParserContainer* container_basket;
};

// ---------------------------------------------------------------------------------
// ----------------------------------- Deleters ------------------------------------
// ---------------------------------------------------------------------------------



void DeleteContainers(ParserContainer* delete_me)
{    
    ParserContainer* prev = delete_me;
    

    while(delete_me != NULL)
    {
        // std::cerr << "- Delete One Node" << std::endl;
        delete_me = delete_me->next_container;

        // M=Makes deletions
        delete prev;

        //Move next
        prev = delete_me;
        // std::cerr << "- Node deleted" << std::endl;
    }
    // std::cerr << "--- Out Delete Nodes" << std::endl;
}

void DeleteNodes(Node* delete_me)
{
    // std::cerr << "--- In Delete Nodes" << std::endl;
    
    Node* prev = delete_me;
    

    while(delete_me != NULL)
    {
        // std::cerr << "- Delete One Node" << std::endl;
        delete_me = delete_me->next;

        // M=Makes deletions
        delete((std::string *)(prev->representative));
        DeleteContainers(prev->container_basket);
        delete prev;

        //Move next
        prev = delete_me;
        // std::cerr << "- Node deleted" << std::endl;
    }
    // std::cerr << "--- Out Delete Nodes" << std::endl;

}

// ---------------------------------------------------------------------------------
// --------------------------- struc print functions -------------------------------
// ---------------------------------------------------------------------------------

void PrintParserContainers(ParserContainer* head)
{
    ParserContainer* ptr = head;
    if(ptr!=NULL)
    {
        std::cerr << "- ";
    }

    while(ptr!=NULL)
    {
        std::cerr << ptr->data;
        ptr = ptr->next_container;
        if(ptr==NULL)
        {
            std::cerr << std::endl;
        }
        else
        {
            std::cerr << std::endl << "- ";
        }
        
    }

}


void PrintNodes(Node* head)
{
    Node* ptr = head;
    std::cerr << "--- Printing Results" << std::endl;
    int counter_for_set_number_printing = 1;
    while(ptr!=NULL)
    {
        std::cerr << "Set #" << counter_for_set_number_printing << ":" << std::endl;
        counter_for_set_number_printing++;
        if(ptr->container_basket!=NULL)
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

void AddDataToBasket(ParserContainer** head, std::string name)
{
    ParserContainer* ptr = *head;
    // std::cout << "- Add to baskets: "<< name << std::endl;
    while(ptr!=NULL)
    {
        // std::cout << "- Enter Loop" << std::endl;

        // std::cout << ptr->data << std::endl;
        ptr = ptr->next_container;
        // std::cout << "- Loop successfully" << std::endl;
    }
    // std::cout << "- Add to container: " << std::endl;

    ptr = new ParserContainer;
    ptr->data = name;
    ptr->next_container = *head;
    *head = ptr;
    // std::cout << "- Successfull addition: " << std::endl;

}

bool CheckAndAdd(Parser* parser, Node** head)
{
    // Iterator:
    Node * ptr = *head;
    void* parser_output = parser->normalize(NULL);

    // std::cout << "comparison iterator" << std::endl;
    while(ptr != NULL)
    {
        // std::cout << "One compariosn" << std::endl;
        // std::cout << "thing one before-> "<< *(std::string *)(ptr->representative) << std::endl;
        // std::cout << "thing two before-> "<< *(std::string *)(parser_output) << std::endl;

        // std::cout << "Two compariosn" << std::endl;

        if( parser->equivalent(ptr->representative, parser_output))
        {
            // std::cout << "Add to container" << std::endl;
            // add to parse basket

            AddDataToBasket(&ptr->container_basket, parser->getName());
            delete((std::string *)(parser_output));
            // std::cout << *(int*)parser_output <<"(my) vs "<< 
            // *(int*)ptr->representative<< "(other)" << std::endl;
            return true;
        }

        ptr = ptr->next;

    }
    ptr = new Node;
    ptr->representative = parser_output;
    ptr->container_basket = new ParserContainer;
    ptr->container_basket->data = parser->getName();
    ptr->container_basket->next_container = NULL;
    ptr->next = *head;
    *head = ptr;
    // add to parse basket

    return false;
}

// ---------------------------------------------------------------------------------
// ----------------------------------- Fuzzing -------------------------------------
// ---------------------------------------------------------------------------------

void DifferentiallyFuzz(Parser** parser_array, int number_of_parsers)
{
    //Creates iteration structure
    Node *head = new Node;
    
    // For error reporting
    bool are_all_parsers_equal = true;

    // std::cout << "----------outer iterator start ------------" << std::endl;

    //First case:
    head->representative = parser_array[0]->normalize(NULL);
    head->next = NULL;
    head->container_basket = new ParserContainer;
    head->container_basket->data = parser_array[0]->getName();
    head->container_basket->next_container = NULL;

    for(int i=1; i<number_of_parsers;i++)
    {
        // std::cout << "----------inner iterator start" << std::endl;

        are_all_parsers_equal = CheckAndAdd(parser_array[i], &head);
    }
    // std::cout << "---------- end iterator  -----------------" << std::endl;
    // PrintNodes(head);
    DeleteNodes(head);
    assert(are_all_parsers_equal);
    
    // std::cout << parser_array[0]->normalize(NULL) << std::endl;
    // std::cout << parser_array[1]->normalize(NULL) << std::endl;
    // assert(parser_array[0]->normalize(NULL)==parser_array[1]->normalize(NULL));
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) 
{
    XmlParser xmlcase;
    YamlParser yamlcase;
    // parser* array_of_parsers[2] = {dynamic_cast<parser*>(&xmlcase), dynamic_cast<parser*>(&yamlcase)};

    // Parser* array_of_parsers[3] = {(Parser*)(&yamlcase), (Parser*)(&xmlcase),(Parser*)(&yamlcase)};
    // DifferentiallyFuzz(array_of_parsers, 3);

    // Parser* array_of_parsers[3] = {(Parser*)(&yamlcase),(Parser*)(&yamlcase), (Parser*)(&xmlcase)};
    // DifferentiallyFuzz(array_of_parsers, 3);

    Parser* array_of_parsers[3] = {(Parser*)(&yamlcase), (Parser*)(&xmlcase),(Parser*)(&yamlcase),};
    DifferentiallyFuzz(array_of_parsers, 3);

    return 0;
}

