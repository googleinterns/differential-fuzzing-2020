rm ./a.out *.o

clang++ ../../libraries/libyaml/src/.libs/libyaml.a ../../libraries/yaml-cpp/build/libyaml-cpp.a -c testmain.cpp -I ./../../libraries/yaml-cpp/include -I ./../../libraries/libyaml/include

clang++ testmain.o ../../libraries/libyaml/src/.libs/libyaml.a ../../libraries/yaml-cpp/build/libyaml-cpp.a

