cmake_minimum_required(VERSION 3.6)
project(Math)

set(CMAKE_CXX_STANDARD 17)

set(MY_TARGET HMathLib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/executable/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/executable/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/executable/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

file(GLOB SOURCE_FILES
    src/*
)
add_executable(${MY_TARGET} ${SOURCE_FILES})

find_package(Eigen3 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)
target_include_directories(${MY_TARGET} PUBLIC ./include
    PRIVATE)

# Link the required libraries
target_link_libraries(${MY_TARGET} PRIVATE
    Eigen3::Eigen   
    GTest::gtest 
    GTest::gtest_main 
    GTest::gmock 
    GTest::gmock_main
)

enable_testing()
add_test(AllTestsInMain main)

