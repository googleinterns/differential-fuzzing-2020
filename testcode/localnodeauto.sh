rm ./autotest *.o


clang++ ../../../libraries/libyaml/src/.libs/libyaml.a ../../../libraries/yaml-cpp/build/libyaml-cpp.a \
./code/nodenormalization/utils.cpp ./code/nodenormalization/libyaml_utils.cpp \
./code/nodenormalization/yamlcpp_utils.cpp ./code/nodenormalization/comparison_utils.cpp -c  \
./code/nodenormalization/automated.cpp -I ../../../libraries/yaml-cpp/include \
-I ./../../../libraries/libyaml/include -I ./code/nodenormalization -I ../

clang++ automated.o comparison_utils.o libyaml_utils.o utils.o yamlcpp_utils.o \
../../../libraries/libyaml/src/.libs/libyaml.a \
../../../libraries/yaml-cpp/build/libyaml-cpp.a \
-I ./code/nodenormalization -I ../ -g -fsanitize=address -o autotest