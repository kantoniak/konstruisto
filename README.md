<img src="graphics/konstruisto.png" width="360"/>

Simple city-builder using OpenGL. For Windows and Linux. See http://konstruisto.com/ for more.

## Building

Just go with `make rebuild run`. To build release configuration: `make rebuild run CONFIG=RELEASE`. `make help` shows possible options.

### Deps for Windows

1. You will need `make`, `clang`, `clang++`, `clang-format` in `PATH`.
2. Install GLEW in `ext/glew-2.0.0`
3. Install GLFW in `ext/glew-3.2.1`
4. Extract `glm` library to `ext/glm`
5. Put `stb_image.h` to `ext/stb/stb/stb_image.h`
6. Build and install `nanovg` (see below)
7. You will need `windres` in `PATH` from `mingw-w64`

### Deps for Linux

1. Install `clang-3.9 lldb-3.9`. Export `clang`, `clang++`, `clang-format` to `PATH`.
2. Install `libglew-dev` (2.0.0): https://launchpad.net/ubuntu/+source/glew
3. Install `libglfw3-dev` (3.2.1): https://launchpad.net/ubuntu/+source/glfw3
4. Extract `glm` library to `ext/glm`
5. Put `stb_image.h` to `ext/stb/stb/stb_image.h`
6. Build and install `nanovg` (see below)
7. You will need `windres` in `PATH` (see `/usr/bin/x86_64-w64-mingw32-windres` from `mingw-w64` package)

### Building `nanovg`

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

## Author and <img src="https://opensource.org/files/osi_symbol.png" height="20" alt="Open Source" /> license

Copyright &copy; 2017 Krzysztof Antoniak

Contents of this repository is licensed under GNU General Public License, version 3.0 (GPL-3.0) with additional term according to the section 7(c) of the license: should you make a derivative of this repository, please do not use the name "Konstruisto", Konstruisto logo or use similar naming for your game what could misguide users. See [LICENSE.txt]() for details.

### Used libraries

* [GLFW 3.2.1](http://www.glfw.org/) under the zlib/libpng license
* [GLEW 2.0.0](http://glew.sourceforge.net/) under the Modified BSD License, the Mesa 3-D License (MIT) and the Khronos License (MIT)
* [GLM 0.9.8.4](http://glm.g-truc.net/0.9.8/index.html) under the MIT License
* [NanoVG](https://github.com/memononen/nanovg) under the zlib/libpng license
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h), a public license image loading library