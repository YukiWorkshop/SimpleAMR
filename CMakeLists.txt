cmake_minimum_required(VERSION 3.8)
project(SimpleAMR)

set(CMAKE_CXX_STANDARD 14)

find_library(AMRNB_LIBRARY
        NAMES opencore-amrnb
        )

find_library(AMRWB_LIBRARY
        NAMES opencore-amrwb
        )

find_library(AMRWBENC_LIBRARY
        NAMES vo-amrwbenc
        )

if(AMRNB_LIBRARY)
    message("Found opencore-amrnb: ${AMRNB_LIBRARY}")
    add_definitions(-DSIMPLEAMR_ENABLE_AMRNB)
endif()

if(AMRWB_LIBRARY)
    message("Found opencore-amrwb: ${AMRWB_LIBRARY}")
    add_definitions(-DSIMPLEAMR_ENABLE_AMRWB)
endif()

if(AMRWBENC_LIBRARY)
    message("Found vo-amrwbenc: ${AMRWBENC_LIBRARY}")
    add_definitions(-DSIMPLEAMR_ENABLE_AMRWBENC)
endif()

add_library(SimpleAMR SimpleAMR.cpp SimpleAMR.hpp)
target_link_libraries(SimpleAMR PUBLIC
        $<$<BOOL:${AMRNB_LIBRARY}>:${AMRNB_LIBRARY}>
        $<$<BOOL:${AMRWB_LIBRARY}>:${AMRWB_LIBRARY}>
        $<$<BOOL:${AMRWBENC_LIBRARY}>:${AMRWBENC_LIBRARY}>)

add_executable(Test_AMRNB_Encode Test_AMRNB_Encode.cpp)
target_link_libraries(Test_AMRNB_Encode PUBLIC SimpleAMR)

add_executable(Test_AMRWB_Encode Test_AMRWB_Encode.cpp)
target_link_libraries(Test_AMRWB_Encode PUBLIC SimpleAMR)