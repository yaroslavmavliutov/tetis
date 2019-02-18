cmake_minimum_required(VERSION 3.12)
project(tetis)

set(CMAKE_CXX_STANDARD 14)

add_executable(tetis ml2.cpp Shape.cpp Shape.h Board.cpp Board.h Tetris.cpp Tetris.h main.cpp main.h Piece.cpp Piece.h Frame.cpp Frame.h InfoPanel.cpp InfoPanel.h MyThread.cpp MyThread.h StartDialog.cpp StartDialog.h)

target_include_directories(tetis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/wx-2.8)