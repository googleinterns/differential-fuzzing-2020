rm *.o testexecutable *.h.gch
bash buildtoybase.sh 
bash buildtoyfuzzparsers.sh

clang++ differentialfuzzer.o toyfuzzstringoutputbase.o toyfuzz7parser.o toyfuzz9parser.o toytests.cpp \
-o toytestexecutable -I ../