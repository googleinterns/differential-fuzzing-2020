clang++ -g -fsanitize=address,fuzzer -c libyamlparser.cpp yamlcppparser.cpp differentialfuzzer.o
