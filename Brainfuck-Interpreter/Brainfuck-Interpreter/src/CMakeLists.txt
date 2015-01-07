include_directories(include)

set(SRC main.cpp args.cpp utils.cpp bfexception.cpp interpreter.cpp)

add_definitions(-std=c++11)

add_executable(BF ${SRC})