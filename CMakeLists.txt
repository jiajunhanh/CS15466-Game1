# Only for IntelliJ, do NOT use CMake to build.
# Use Maekfile.js instead.
# More information in NEST.md file.

cmake_minimum_required(VERSION 3.24)
project(CS15466_Game1)

set(CMAKE_CXX_STANDARD 17)
set(NEST_LIBS ../nest-libs/windows)

include_directories(.
        ${NEST_LIBS}/SDL2/include
        ${NEST_LIBS}/glm/include
        ${NEST_LIBS}/libpng/include)

add_executable(CS15466_Game1
        asset_pipeline.cpp
        data_path.cpp
        GL.cpp
        gl_compile_program.cpp
        Level.cpp
        Load.cpp
        load_save_png.cpp
        main.cpp
        Mode.cpp
        PlayMode.cpp
        PPU466.cpp)
