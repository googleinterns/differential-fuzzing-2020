rm ./autotest *.o


clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c ./code/stringnormalizationtests/testutils.cpp ./code/stringnormalizationtests/automatedyamlexamplestest.cpp -I ../../../libraries/yaml-cpp/include \
-I ./../../../libraries/libyaml/include -I ./code/stringnormalizationtests -I ../

clang++ automatedyamlexamplestest.o testutils.o ../../../libraries/libyaml/src/.libs/libyaml.a \
../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code/stringnormalizationtests -I ../ -g -fsanitize=address -o autotest