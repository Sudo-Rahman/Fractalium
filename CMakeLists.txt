cmake_minimum_required(VERSION 3.26)
project(Fractalium)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(Boost COMPONENTS thread mpi REQUIRED)


file(GLOB_RECURSE SRC
        src/*.cpp
        src/*.h
        src/*.hpp
)

include_directories(src)

add_executable(Fractalium ${SRC} main.cpp)

target_compile_options(Fractalium PRIVATE -O3)
target_link_libraries(Fractalium
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Boost::mpi
        Boost::thread
)
