rm ./autotest *.o


clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c ./code/nodenormalization/utils.cpp ./code/nodenormalization/automated.cpp -I ../../../libraries/yaml-cpp/include \
-I ./../../../libraries/libyaml/include -I ./code/nodenormalization -I ../

clang++ automated.o utils.o ../../../libraries/libyaml/src/.libs/libyaml.a \
../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code/nodenormalization -I ../ -g -fsanitize=address -o autotest