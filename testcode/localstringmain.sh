rm ./manualtest *.o

clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
./code/stringnormalizationtests/testutils.cpp -c ./code/stringnormalizationtests/testmain.cpp -I ../../../libraries/yaml-cpp/include \
-I ./../../../libraries/libyaml/include -I ./code/stringnormalizationtests -I ../

clang++ testmain.o testutils.o ../../../libraries/libyaml/src/.libs/libyaml.a \
../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code/stringnormalizationtests -I ../ -g -fsanitize=address -o manualtest


