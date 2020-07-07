rm *.o testexecutable *.h.gch
bash buildbase.sh 
bash buildyamlxmlparsers.sh 
clang++ testfuzz.cpp differentialfuzzer.o  xmlparser.o  yamlparser.o -o testexecutable -g -fsanitize=address,fuzzer
