rm ./a.out *.h.gch crash-da39a3ee5e6b4b0d3255bfef95601890afd80709 
clang++ -g -fsanitize=address,fuzzer fuzzer.cpp parser.h xmlparser.cpp xmlparser.h yamlparser.cpp yamlparser.h
