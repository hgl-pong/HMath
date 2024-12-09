set(ENABLE_DELAUNAY2D_EXAMPLE true)
set(ENABLE_IMAGE_UTILS_EXAMPLE true)
set(ENABLE_EROISON_EXAMPLE true)

if(${ENABLE_DELAUNAY2D_EXAMPLE})
set(DELAUNAY_2D_EXAMPLE Delaunay2DExample)
file(GLOB DELAUNAY_2D_EXAMPLE_SOURCE_FILES
    delaunay2dExample.cpp
)
add_executable(${DELAUNAY_2D_EXAMPLE} ${DELAUNAY_2D_EXAMPLE_SOURCE_FILES})
find_package(Eigen3 CONFIG REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(${DELAUNAY_2D_EXAMPLE} PUBLIC ./include
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${DELAUNAY_2D_EXAMPLE} PRIVATE
    Eigen3::Eigen
    GLUT::GLUT
)
if(ENABLE_PARALLEL)
add_definitions(-DUSE_TBB)
find_package(TBB CONFIG REQUIRED)
target_link_libraries(${DELAUNAY_2D_EXAMPLE} PRIVATE TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)
endif()

endif()

if(${ENABLE_IMAGE_UTILS_EXAMPLE})
set(IMAGE_UTILS_EXAMPLE ImageUtilsExample)
file(GLOB IMAGE_UTILS_EXAMPLE_SOURCE_FILES
    ImageUtilsExample.cpp
)
add_executable(${IMAGE_UTILS_EXAMPLE} ${IMAGE_UTILS_EXAMPLE_SOURCE_FILES})
find_package(Eigen3 CONFIG REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(${IMAGE_UTILS_EXAMPLE} PUBLIC ./include
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${IMAGE_UTILS_EXAMPLE} PRIVATE
    Eigen3::Eigen
    GLUT::GLUT
)
if(ENABLE_PARALLEL)
add_definitions(-DUSE_TBB)
find_package(TBB CONFIG REQUIRED)
target_link_libraries(${IMAGE_UTILS_EXAMPLE} PRIVATE TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)
endif()
endif()

if(${ENABLE_EROISON_EXAMPLE})
set(EROISON_EXAMPLE EroisonExample)
file(GLOB EROISON_EXAMPLE_SOURCE_FILES
    FastNoiseLite.h
    erosionExample.cpp
)
add_executable(${EROISON_EXAMPLE} ${EROISON_EXAMPLE_SOURCE_FILES})
find_package(Eigen3 CONFIG REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(${EROISON_EXAMPLE} PUBLIC ./include
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(${EROISON_EXAMPLE} PRIVATE
    Eigen3::Eigen
    GLUT::GLUT
)
if(ENABLE_PARALLEL)
add_definitions(-DUSE_TBB)
find_package(TBB CONFIG REQUIRED)
target_link_libraries(${EROISON_EXAMPLE} PRIVATE TBB::tbb TBB::tbbmalloc TBB::tbbmalloc_proxy)
endif()
endif()