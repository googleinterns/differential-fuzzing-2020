Directory presents test cases for the DifferentialFuzzer. Tests
show how the DifferentialFuzzer handles different situations. 
.sh files fonund in this directory can be used to build the proper,
 files used for testing and running the code

Code can be found inside of the 'code' directory, where:

- ./code/toyfuzzstringoutputbase.cpp and toyfuzzstringoutputbase.h: 
Offers generic use for classes that run with strings, and
offers a generic mutable class for strings

- ./code/toyfuzzintparser.h and toyfuzzintparser.cpp:
Show parsing of data throw a class that primarily deals with
the integer tipe

- ./code/toyfuzzswitchparser.cpp and toyfuzzswitchparser.h: 
Class where output changes depending on input

