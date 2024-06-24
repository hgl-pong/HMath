set(ENABLE_DELAUNAY2D_EXAMPLE true)

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
endif()