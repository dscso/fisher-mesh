cmake_minimum_required(VERSION 3.22)
project(fisher_mesh C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_executable(fisher_mesh src/main.c src/fisher.c include/fisher.h src/util.c include/util.h)
