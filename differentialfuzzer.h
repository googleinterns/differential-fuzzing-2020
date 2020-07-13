#ifndef DIFFERENTIALFUZZER_H
#define DIFFERENTIALFUZZER_H

#include "baseparser.h"

namespace differential_fuzzer
{
    namespace parser
    {
        struct AssociatedParserName
        {
            std::string name;
            AssociatedParserName* next_container;
        };

        // Linked list used to transverse between previous sets of parsers.
        // One representative for each set is saved for future comparisons.
        struct EquivalenceParserOutputs
        {
            EquivalenceParserOutputs* next;
            differential_parser::ParserOutput* representative;
            AssociatedParserName* container_basket;
        };

        void DeleteAssociatedParserName(AssociatedParserName*);

        void DeleteEquivalenceParserOutputs(EquivalenceParserOutputs*);

        void PrintAssociatedParserName(AssociatedParserName*);

        void PrintEquivalenceParserOutputs(EquivalenceParserOutputs*);

        void AddToAssociatedParserName(AssociatedParserName**, std::string);

        bool CheckAndAdd(differential_parser::Parser*, EquivalenceParserOutputs**, const uint8_t *, int);
    }

    namespace fuzzer
    {
        void DifferentiallyFuzz(differential_parser::Parser**, int, const uint8_t *, int);
    }
}

#endif