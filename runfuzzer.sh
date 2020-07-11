rm *.o testexecutable *.h.gch
bash buildbase.sh 
bash buildyamlxmlparsers.sh 
clang++ testfuzz.cpp differentialfuzzer.o  libyamlparser.o  yamlcppparser.o -o testexecutable -g -fsanitize=address,fuzzer
