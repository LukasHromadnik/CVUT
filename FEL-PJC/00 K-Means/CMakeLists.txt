cmake_minimum_required(VERSION 3.5)
project(K-Means LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.68 COMPONENTS system iostreams filesystem)

if ( Boost_FOUND )
    include_directories(${Boost_INCLUDE_DIRS})

    add_executable(kmeans
        Configuration.cpp
        Configuration.hpp
        main.cpp
    )

    if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
        target_link_libraries(kmeans ${Boost_LIBRARIES})
        target_compile_options( kmeans PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic ${BOOST_COMPILE_OPTIONS})
    endif()
    if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
        target_link_libraries(kmeans ${Boost_LIBRARIES})
        target_compile_options( kmeans PRIVATE /W4 )
    endif()
endif()
