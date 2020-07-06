clang++ -g -fsanitize=address,fuzzer -c xmlparser.cpp xmlparser.h yamlparser.cpp yamlparser.h differentialfuzzer.o
