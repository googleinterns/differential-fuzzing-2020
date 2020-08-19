#include "differential_fuzzer.h"

#ifdef DEBUG_MODE
    #define TEST_NODE_PPRINT(head) PrintEquivalenceNormalizedOutputs(head)
#else
    #define TEST_NODE_PPRINT(head)
#endif