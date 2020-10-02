# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
