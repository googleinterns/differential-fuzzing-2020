rm *.o testexecutable *.h.gch
bash buildbase.sh 
bash buildyamlxmlparsers.sh 
clang++ testfuzz.cpp *.o -o testexecutable -g -fsanitize=address,fuzzer
