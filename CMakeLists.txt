cmake_minimum_required(VERSION 3.25)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project(PascalServer CXX)
set(CMAKE_CXX_STANDARD 17)
add_library(PascalServer
        PascalServer/src/Core/Log.cpp
        PascalServer/src/Core/Log.h
        PascalServer/src/pspch.h
        PascalServer/src/pspch.cpp)
target_include_directories(PascalServer PRIVATE
        PascalServer/src
        PascalServer/vendor/spdlog/include)

target_compile_options(PascalServer PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>

    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)

target_precompile_headers(PascalServer PRIVATE PascalServer/src/pspch.h)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DPS_DEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DPS_RELEASE)
endif ()

project(Sandbox CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(Sandbox Sandbox/src/main.cpp)
add_dependencies(Sandbox PascalServer)
target_include_directories(Sandbox PRIVATE
        Sandbox/src
        PascalServer/src
        PascalServer/vendor/spdlog/include)

target_link_libraries(Sandbox PascalServer)

target_compile_options(Sandbox PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>

    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
)


