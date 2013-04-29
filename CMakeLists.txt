cmake_minimum_required(VERSION 2.8)
include(FRCDeploy)

project(Leonidas)
find_package(WPILib)
include_directories(${WPILib_INCLUDE_DIRS})

set(DIRS
    AutonomousModes/*
    Kinect/*
    LiveGrapherHost/*
    Subsystems/*
    SFMLNetwork/*
    SFMLSystem/*
)

file(GLOB SRC *.cpp *.c)

foreach(ELEM ${DIRS})
    file(GLOB_RECURSE FILES ${ELEM}.cpp ${ELEM}.c)
    list(APPEND SRC ${FILES})
endforeach(ELEM)

add_executable(Leonidas.out ${SRC})

target_link_libraries(Leonidas.out ${WPILib_LIBRARY})

add_make_deploy(Leonidas.out 10.35.12.2)
