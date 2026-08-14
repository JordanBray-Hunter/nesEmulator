cmake_minimum_required(VERSION 3.16)
project(nes_emulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)


FetchContent_MakeAvailable(raylib)


add_executable(nes_emu
    src/main.c
    src/cpu.c
    src/ppu.c
    src/bus.c
    src/cartridge.c
)

target_include_directories(nes_emu PRIVATE include)

target_link_libraries(nes_emu PRIVATE raylib)

