cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(simonsays C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(src)
add_subdirectory(libs/pico-neotrellis) #FROM: git submodule add https://github.com/dcooperdalrymple/pico-neotrellis.git
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../lib/abi_lib build)
