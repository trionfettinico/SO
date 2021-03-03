cmake_minimum_required(VERSION 3.15)
project(playfair C)

set(CMAKE_C_STANDARD 11)

add_executable(playfair main.c file.c file.h keyfile.c playfair.c keyfile.h playfair.h)