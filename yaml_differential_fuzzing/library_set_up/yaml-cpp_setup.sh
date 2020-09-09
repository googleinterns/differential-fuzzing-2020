#File makes setup related to yaml-cpp.

# Clone from git:
git clone https://github.com/jbeder/yaml-cpp.git
cd yaml-cpp

# Version worked on:
git checkout 97d1ae547cc9935cf851801ba0a247b4be2191de

# Build:
mkdir build
cd build
cmake ..
make

# You can find libyaml-cpp.a in the yaml-cpp/build
