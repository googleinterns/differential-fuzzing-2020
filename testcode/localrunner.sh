rm ./a.out *.o

clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
./code/testutils.cpp -c ./code/testmain.cpp -I ../../../libraries/yaml-cpp/include \
-I ./../../../libraries/libyaml/include -I ./code -I ../

clang++ testmain.o testutils.o ../../../libraries/libyaml/src/.libs/libyaml.a \
../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code -I ../ -g -fsanitize=address -o manualtest


