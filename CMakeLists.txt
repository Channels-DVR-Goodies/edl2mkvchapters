cmake_minimum_required(VERSION 3.17)
project(edl2mkvchapters C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable( edl2mkvchapters
                edl2mkvchapters.c edl2mkvchapters.h)
