rm ./autotest *.o

clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c ./code/testutils.cpp ./code/automatedyamlexamplestest.cpp -I ../../../libraries/yaml-cpp/include \
-I ./../../../libraries/libyaml/include -I ./code -I ../

clang++ automatedyamlexamplestest.o ../../../libraries/libyaml/src/.libs/libyaml.a \
../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../ -g -fsanitize=address -o autotest