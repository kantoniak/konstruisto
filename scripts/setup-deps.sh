#!/bin/sh

# This script setups deps for Linux systems.

VENDOR="./vendor/"
BUILD_TYPE="linux"
#BUILD_TYPE="windows"

echo "Setting up Konstruisto dependencies for $BUILD_TYPE..."
echo "Dependencies directory: ${VENDOR}"

# 1. Create directory
mkdir -p "${VENDOR}"

# 2. Install packages
sudo apt-get install clang-9 clang-format-9 clang-tidy-9 clang-tools-9 libglfw3-dev mingw32-binutils libassimp-dev

# 3. Download GLM
wget https://github.com/g-truc/glm/releases/download/0.9.9.6/glm-0.9.9.6.zip --no-check-certificate -O glm-0.9.8.4.zip &&
unzip glm-0.9.8.4.zip -d "${VENDOR}" &&
mv "${VENDOR}/glm" "${VENDOR}/glm-0.9.9.6" &&
rm glm-0.9.8.4.zip

# 4. Download cereal
wget https://github.com/USCiLab/cereal/archive/v1.3.0.zip --no-check-certificate -O cereal-1.3.0.zip &&
unzip cereal-1.3.0.zip -d "${VENDOR}" &&
rm cereal-1.3.0.zip

# 5. Download stb_image
mkdir -p "${VENDOR}/stb/stb/" &&
wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h --no-check-certificate -P "${VENDOR}/stb/stb/"

# 6. Download and build NanoVG
git clone git@github.com:memononen/nanovg.git "${VENDOR}/nanovg" &&
cd "${VENDOR}/nanovg" &&
premake4 --cc=gcc gmake &&
mv "build/" "build-$BUILD_TYPE/" &&
cd "build-$BUILD_TYPE/" &&
make config=release64 nanovg

# Others
echo "Done."
