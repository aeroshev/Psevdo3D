cmake_minimum_required(VERSION 3.15)
project(Psevdo3D)

add_library(ncurses STATIC IMPORTED)
set_property(TARGET ncurses PROPERTY IMPORTED_LOCATION /usr/local/Cellar/ncurses)
target_link_libraries(${TARGET} ncurses)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES TerminalFPS.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -lncurses")

add_executable(Psevdo3D ${SOURCES})
