rm ./manualtest *.o

clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
./code/nodenormalization/utils.cpp -c ./code/nodenormalization/testmain.cpp -I ../../../libraries/yaml-cpp/include \
-I ./../../../libraries/libyaml/include -I ./code/nodenormalization -I ../

clang++ testmain.o utils.o ../../../libraries/libyaml/src/.libs/libyaml.a \
../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code/nodenormalization -I ../ -g -fsanitize=address -o manualtest


