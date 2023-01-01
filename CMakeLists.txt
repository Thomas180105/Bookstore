cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp src/error.cpp src/linkedList.h src/strScanner.h src/strScanner.cpp src/user.cpp src/user.h src/book.cpp src/diary.cpp src/diary.h)
