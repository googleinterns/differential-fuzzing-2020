rm ./a.out *.o

clang++ ../../libraries/libyaml/src/.libs/libyaml.a ../../libraries/yaml-cpp/build/libyaml-cpp.a \
-c ./testcode/testmain.cpp -I ./../../libraries/yaml-cpp/include \
-I ./../../libraries/libyaml/include -I ./testcode -I ./

clang++ testmain.o ../../libraries/libyaml/src/.libs/libyaml.a \
../../libraries/yaml-cpp/build/libyaml-cpp.a -I ./testcode -g -fsanitize=address


