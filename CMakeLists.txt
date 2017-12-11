cmake_minimum_required(VERSION 3.8)
project(saboteur_server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-pthread")

set(SOURCE_FILES main.cpp SaboteurGame.cpp SaboteurGame.h Client.cpp Client.h Card.cpp Card.h Board.cpp Board.h)
add_executable(saboteur_server ${SOURCE_FILES})