#File makes setup related to yaml-cpp.

# Clone from git:
git clone https://github.com/jbeder/yaml-cpp.git
cd yaml-cpp

# Build:
mkdir build
cd build
cmake ..
make

# You can find libyaml-cpp.a in the yaml-cpp/build
