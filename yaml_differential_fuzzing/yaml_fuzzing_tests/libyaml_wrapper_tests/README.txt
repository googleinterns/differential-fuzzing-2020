Directory contains testing for the yaml-cpp wrapper. 
The code found in code/test_yamlcpp_wrapper_check.cpp can be build
using 'bash create_yamlcpp_wrapper_check_test.sh'. The executable 
can then be ran to see if the wrapper works as intended with a few
examples. Mainly we do SmokeTesting, and then we check that
the internal comparison tool works as intended

The code found in code/test_yamlcpp_parser_fuzz.cpp can be build
using 'bash create_yamlcpp_parser_sanity_test.sh', and executed using
'bash run_yamlcpp_parser_sanity_test.sh' which contains proper settings.
This will executed differential fuzzing of the class against itself,
and serves as a sanity test. The settings are necessary due to a 
bug in YAML::Node that can be easily triggered with libfuzzer