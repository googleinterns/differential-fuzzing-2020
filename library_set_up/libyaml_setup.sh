#File makes setup related to libyaml.

# Get github:
git clone https://github.com/yaml/libyaml
cd libyaml

# Version worked on:
git checkout acd6f6f014c25e46363e718381e0b35205df2d83

# build:
./bootstrap
./configure
make

