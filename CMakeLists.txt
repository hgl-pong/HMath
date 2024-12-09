cmake_minimum_required(VERSION 3.6)
project(Math)

set(CMAKE_CXX_STANDARD 17)

set(MATH_LIB HMathLib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/executable/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/executable/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/executable/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

file(GLOB SOURCE_FILES
    src/*
)
add_executable(${MATH_LIB} ${SOURCE_FILES})

find_package(Eigen3 CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)


set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)
target_include_directories(${MATH_LIB} PUBLIC ./include
    PRIVATE)

# Link the required libraries
target_link_libraries(${MATH_LIB} PRIVATE
    Eigen3::Eigen   
    GTest::gtest 
    GTest::gtest_main 
    GTest::gmock 
    GTest::gmock_main
)
set(ENABLE_PARALLEL ON)
if(ENABLE_PARALLEL)
add_definitions(-DUSE_TBB)
find_package(TBB CONFIG REQUIRED)
target_link_libraries(${MATH_LIB} PRIVATE TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)
endif()

enable_testing()
add_test(AllTestsInMain main)

add_subdirectory(examples)

