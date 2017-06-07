#!/bin/sh

# You will have to setup some stuff by yourself!

# GLM
wget https://github.com/g-truc/glm/releases/download/0.9.8.4/glm-0.9.8.4.zip --no-check-certificate -O glm-0.9.8.4.zip && unzip glm-0.9.8.4.zip -d ./ && rm glm-0.9.8.4.zip

# stb_image
mkdir -p stb/stb/ && wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h --no-check-certificate -P stb/stb/

# Google Test
wget https://github.com/google/googletest/archive/release-1.8.0.zip --no-check-certificate && unzip release-1.8.0 -d ./ && rm release-1.8.0
mkdir -p googletest-release-1.8.0/googletest/lib/ && cd googletest-release-1.8.0/googletest/lib/
clang++ -isystem ../include -I../ -pthread -c "../src/gtest-all.cc"
ar -rv libgtest.a gtest-all.o
cd -

# Others
echo "Things you have to setup by yourself: GLFW, GLEW, NanoVG"