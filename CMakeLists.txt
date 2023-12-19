cmake_minimum_required(VERSION 3.26)
project(Fractalium)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -funroll-loops")
set(CMAKE_BUILD_TYPE Release)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS
            Core
            Gui
            Widgets
            REQUIRED)
endif ()

find_package(Boost COMPONENTS mpi REQUIRED)

file(GLOB_RECURSE SRC
        src/*.cpp
        src/*.h
        src/*.hpp
)

include_directories(src)

add_executable(Fractalium ${SRC} main.cpp)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(ProcessorCount)
ProcessorCount(N)
if (NOT N EQUAL 0)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif ()

target_link_libraries(Fractalium
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Boost::mpi
)
