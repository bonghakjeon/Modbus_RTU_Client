cmake_minimum_required(VERSION 3.16)
project(Mod_Master C)

set(CMAKE_C_STANDARD 11)

add_executable(Mod_Master master.c mod.h mod.c)