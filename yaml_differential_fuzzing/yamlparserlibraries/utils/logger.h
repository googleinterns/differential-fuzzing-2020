#include <iostream>

#ifdef DEBUG_MODE
    #define TEST_PPRINT(head) fprintf(stderr, head);
#else
    #define TEST_PPRINT(head)
#endif