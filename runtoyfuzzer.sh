rm *.o testexecutable *.h.gch
bash buildtoybase.sh 
bash buildtoyfuzzparsers.sh

clang++ differentialfuzzer.o toyfuzz1parser.o toyfuzz2parser.o  toyfuzz3parser.o toyfuzz4parser.o \
toyfuzz.cpp -o toytestexecutable