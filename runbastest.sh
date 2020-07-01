rm ./a.out *.h.gch 
clang++ -g -fsanitize=address,fuzzer fuzzer.cpp parser.h xmlparser.cpp xmlparser.h yamlparser.cpp yamlparser.h
