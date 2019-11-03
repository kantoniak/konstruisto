<img src="graphics/konstruisto.png" width="360"/>

<p align="center">
  <img src="readme/screenshot.png" align="center" alt="Project preview" />
</p>

Simple city-builder using OpenGL. For Windows and Linux. See http://konstruisto.com/ for more.

## Building

There are two building toolchains:

1. Linux/Windows GNU headers using Makefile and Clang
1. Windows MSVC build using Visual Studio and Clang

Only x64 builds are supported. MSVC is preferred for Windows. Windows GNU version is no longer maintained and will be removed.

### Building on Linux

1. Set up dependencies running `scripts/setup-deps.sh` from the main directory:
    ````
    git clone git@github.com:kantoniak/konstruisto.git konstruisto &&
    cd konstruisto &&
    chmod +x ./scripts/setup-deps.sh &&
    sudo ./scripts/setup-deps.sh &&
    cd -
    ````
1. Run `make build run`

To build release configuration use `make rebuild run CONFIG=RELEASE`. Command `make help` shows all possible options.

### Building on Windows

1. Set up dependencies running `scripts/setup-deps.ps1` in Powershell from the main directory:
    ```
    git clone git@github.com:kantoniak/konstruisto.git konstruisto
    Set-Location konstruisto
    .\scripts\setup-deps.ps1
    ```
1. Build NanoVG release libraries:
    1. Open solution `ext\nanovg\build-msvc\nanovg.sln`
    1. Build project `nanovg` for Release/x64.
1. Open Visual Studio solution and build project.

Please note that only x64 builds are supported.

#### (Deprecated) Windows build with MinGW headers

You can use `scripts/setup-deps.ps1` to set up the dependencies.

1. You will need `make`, `clang`, `clang++`, `clang-format` in `PATH`.
1. You will need `windres` in `PATH` from `mingw-w64`
1. Install GLEW in `ext/glew-2.1.0`
1. Install GLFW in `ext/glew-3.3`
1. Extract `glm` library to `ext/glm-0.9.9.6`
1. Extract `cereal` library to `ext/cereal-1.3.0`
1. Put `stb_image.h` to `ext/stb/stb/stb_image.h`
1. Build and install `nanovg` (see below)

### Building `nanovg`

You will need `premake5` [(download)](https://premake.github.io/download.html).

1. Extract repo into `ext/nanovg`.
1. Build x64 release:

    ```
    cd ext/nanovg
    premake [vs2019|--cc=gcc gmake]
    mv build build-[msvc|windows|linux]
    cd build-[msvc|windows|linux]
    make config=release64 nanovg
    ```

## License

Copyright &copy; 2017-2019 Krzysztof Antoniak

Contents of this repository is licensed under GNU General Public License, version 3.0 (GPL-3.0) with additional term according to the section 7(c) of the license: should you make a derivative of this repository, please do not use the name "Konstruisto", Konstruisto logo or use similar naming for your game what could misguide users. See [LICENSE.txt](LICENSE.txt) for details.

### Used libraries

* [GLFW 3.3](http://www.glfw.org/) under the zlib/libpng license
* [GLEW 2.1.0](http://glew.sourceforge.net/) under the Modified BSD License, the Mesa 3-D License (MIT) and the Khronos License (MIT)
* [GLM 0.9.9.6](http://glm.g-truc.net/0.9.9.6/index.html) under the MIT License
* [Cereal 1.3.0](http://uscilab.github.io/cereal/) under BSD license
* [NanoVG](https://github.com/memononen/nanovg) under the zlib/libpng license
* [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h), a public license image loading library
