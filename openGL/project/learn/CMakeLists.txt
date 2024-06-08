cmake_minimum_required(VERSION 3.0)

project(learn)

set(GLFW_INCLUDE  ${PROJECT_SOURCE_DIR}/../../3th/glfw/include)
set(GLFW_LIB ${PROJECT_SOURCE_DIR}/../../3th/glfw/lib) 
set(GLEW_INCLUDE ${PROJECT_SOURCE_DIR}/../../3th/glew/include)
set(GLEW_LIB ${PROJECT_SOURCE_DIR}/../../3th/glew/lib64)
include_directories(${GLFW_INCLUDE} ${GLEW_INCLUDE})
link_directories(${GLFW_LIB} ${GLEW_LIB})
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/test.cpp)
target_link_libraries(${PROJECT_NAME} glfw3 GLEW GL)

