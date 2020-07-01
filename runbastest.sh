rm ./a.out *.h.gch 
clang++ -g -fsanitize=address,fuzzer testfuzz.cpp differentialfuzzer.cpp parser.h xmlparser.cpp xmlparser.h yamlparser.cpp yamlparser.h
