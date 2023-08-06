cmake_minimum_required(VERSION 3.25)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

project(PascalServer CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(PascalServer
        ${PROJECT_NAME}/src/Core/Log.cpp
        ${PROJECT_NAME}/src/Core/Log.h
        ${PROJECT_NAME}/src/Core/Assert.h
        ${PROJECT_NAME}/src/Core/Base.h
        ${PROJECT_NAME}/src/Core/Application.h
        ${PROJECT_NAME}/src/Core/Application.cpp
        ${PROJECT_NAME}/src/pspch.h
        ${PROJECT_NAME}/src/pspch.cpp
        ${PROJECT_NAME}/src/Server/HttpServer.h
        ${PROJECT_NAME}/src/Server/HttpServer.cpp
        ${PROJECT_NAME}/src/Core/Poller.h
        ${PROJECT_NAME}/src/Core/Poller.cpp
        ${PROJECT_NAME}/src/Platform/Pollers/EPollPoller.h
        ${PROJECT_NAME}/src/Platform/Pollers/EPollPoller.cpp
        ${PROJECT_NAME}/src/Core/EventDescription.h
        ${PROJECT_NAME}/src/Core/EventDescription.cpp
        ${PROJECT_NAME}/src/Network/Socket.h
        ${PROJECT_NAME}/src/Network/Socket.cpp
        ${PROJECT_NAME}/src/Network/INetAddress.h
        ${PROJECT_NAME}/src/Network/INetAddress.cpp
        ${PROJECT_NAME}/src/Core/EventLoop.h
        ${PROJECT_NAME}/src/Core/EventLoop.cpp)

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

add_executable(Sandbox 
    Sandbox/src/main.cpp
    Sandbox/src/Tests/TestUtils.h
    Sandbox/src/Tests/INetAddressTest.h)

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


