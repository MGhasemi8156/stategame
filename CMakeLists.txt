cmake_minimum_required(VERSION 3.0)
project(state C)
set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCE "src/*.c" "src/*.h")
add_executable(state "${SOURCE}")

include_directories("/usr/include/SDL2", 
                    "./src/")

target_link_libraries(state m SDL2 SDL2_gfx SDL_ttf)
