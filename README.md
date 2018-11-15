<img src="graphics/konstruisto.png" width="360"/>

Simple city-builder using OpenGL. For Windows and Linux. See http://konstruisto.com/ for more.

## Building

1. Setup dependencies (below)
2. Run `make rebuild run`

To build release configuration: `make rebuild run CONFIG=RELEASE`. Command `make help` shows all possible options.

### Dependencies

#### Windows dependencies

1. You will need `make`, `clang`, `clang++`, `clang-format` in `PATH`.
2. Install GLEW in `ext/glew-2.0.0`
3. Install GLFW in `ext/glew-3.2.1`
4. Extract `glm` library to `ext/glm`
5. Put `stb_image.h` to `ext/stb/stb/stb_image.h`
6. Build and install `nanovg` (see below)
7. You will need `windres` in `PATH` from `mingw-w64`

##### Building `nanovg`

You will need `premake4` [(download)](https://premake.github.io/download.html).

1. Extract repo into `ext/nanovg`.
2. Build x64 release:

    ```
    cd ext/nanovg
    premake --cc=gcc gmake
    mv build build-[windows|linux]
    cd build-[windows|linux]
    make config=release64 nanovg
    ```

#### Linux dependencies

You can just run `setup-deps.sh`, it will setup everything for you. If you want to do everything by hand, here's the list of dependencies:

1. Install `clang-3.9 lldb-3.9`. Export `clang`, `clang++`, `clang-format` to `PATH`.
2. Install `libglew-dev` (2.0.0): https://launchpad.net/ubuntu/+source/glew
3. Install `libglfw3-dev` (3.2.1): https://launchpad.net/ubuntu/+source/glfw3
4. Download `glm` and extract to `ext/glm`
    ```
    wget https://github.com/g-truc/glm/releases/download/0.9.8.4/glm-0.9.8.4.zip &&
    unzip glm-0.9.8.4.zip -d ext/ &&
    rm glm-0.9.8.4.zip
    ```
5. Download `stb_image.h` to `ext/stb/stb/stb_image.h`
    ```
    mkdir -p ext/stb/stb/ &&
    wget https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -P ext/stb/stb/
    ```
6. Build and install `nanovg` (you will need `premake4`)
    ```
    git clone git@github.com:memononen/nanovg.git ext/nanovg &&
    cd ext/nanovg &&
    premake4 --cc=gcc gmake &&
    mv build/ build-linux/ &&
    cd build-linux/ &&
    make config=release64 nanovg
    ```

## Author and <img src="https://opensource.org/files/osi_symbol.png" height="20" alt="Open Source" /> license

Copyright &copy; 2017 Krzysztof Antoniak

Contents of this repository is licensed under GNU General Public License, version 3.0 (GPL-3.0) with additional term according to the section 7(c) of the license: should you make a derivative of this repository, please do not use the name "Konstruisto", Konstruisto logo or use similar naming for your game what could misguide users. See [LICENSE.txt]() for details.

### Used libraries

* [GLFW 3.2.1](http://www.glfw.org/) under the zlib/libpng license
* [GLEW 2.0.0](http://glew.sourceforge.net/) under the Modified BSD License, the Mesa 3-D License (MIT) and the Khronos License (MIT)
* [GLM 0.9.8.4](http://glm.g-truc.net/0.9.8/index.html) under the MIT License
* [NanoVG](https://github.com/memononen/nanovg) under the zlib/libpng license
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h), a public license image loading library
