cmake_minimum_required(VERSION 3.0)

project(myOpengl)

set(GLFW_INCLUDE  ${PROJECT_SOURCE_DIR}/../3th/glfw/include)
set(GLFW_LIB ${PROJECT_SOURCE_DIR}/../3th/glfw/lib) 
set(GLAD_INCLUDE ${PROJECT_SOURCE_DIR}/../3th/glad/include)
set(IMAGE_INCLUDE ${PROJECT_SOURCE_DIR}/../3th/img)
set(GLM_INCLUDE ${PROJECT_SOURCE_DIR}/../3th/glm)

include_directories(${GLFW_INCLUDE} ${GLAD_INCLUDE} ${IMAGE_INCLUDE} ${GLM_INCLUDE})
link_directories(${GLFW_LIB})


add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/application.cpp ${PROJECT_SOURCE_DIR}/../3th/glad/src/glad.c)
target_link_libraries(${PROJECT_NAME} glfw3 GL)

