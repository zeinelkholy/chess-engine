cmake_minimum_required(VERSION 3.16)
project(ChessEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(chess-engine src/main.cpp 
src/Board.cpp 
src/Notation.cpp
src/MoveGenerator.cpp)
