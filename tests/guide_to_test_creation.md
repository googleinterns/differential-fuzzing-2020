This file presents a guide for creating tests for the differential fuzzer classes when you 
implement your own. A good sample of how to test your own wrappers can be seen in the 
*./tamlcpp_wrapper_tests*.

*./test_yamlcpp_wrapper_check.cpp* contains methods that make smoke testing, and logical testing
of the equivalence method.

While this will change from application to application, for smoke testing you can follow these
steps:

1. Create wrapper

2. Parse inout through

3. Analyze results

For the logical test using 2 versions of the same wrapper:

1. Parse data through each parse (recomended use of a helper method)

2. Analyze tipes of results