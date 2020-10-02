// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DIFFERENTIAL_FUZZER_H
#define DIFFERENTIAL_FUZZER_H

#include "base_parser.h"

namespace differential_fuzzer
{
    namespace parser
    {
        struct AssociatedParserName
        {
            std::string name;
            AssociatedParserName* next_parser;
        };

        // Linked list used to transverse between previous sets of parsers.
        // One representative for each set is saved for future comparisons.
        struct EquivalenceNormalizedOutputs
        {
            EquivalenceNormalizedOutputs* next;
            differential_parser::NormalizedOutput* representative;
            AssociatedParserName* container_basket;
        };

        void DeleteAssociatedParserName(AssociatedParserName* delete_me);
        void DeleteEquivalenceNormalizedOutputs(EquivalenceNormalizedOutputs* delete_me);
        void PrintParserList(AssociatedParserName* head);
        void PrintEquivalenceNormalizedOutputs(EquivalenceNormalizedOutputs* head);
        void AddToAssociatedParserName(AssociatedParserName** head, std::string name);
        bool CheckAndAdd(differential_parser::Parser* parser, EquivalenceNormalizedOutputs** head, 
            const uint8_t* input_data, size_t size_of_input);
    }// namespace parser

    namespace fuzzer
    {
        bool DifferentiallyFuzz(differential_parser::Parser** parser_array, int number_of_parsers, 
            const uint8_t* input_data, size_t size_of_input);
    } // namespace fuzzer
} // namespace differential_fuzzer

#endif