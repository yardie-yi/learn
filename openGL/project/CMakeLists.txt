cmake_minimum_required(VERSION 3.0)

project(myOpengl)

set(GLFW_INCLUDE  ${PROJECT_SOURCE_DIR}/../3th/glfw/include)
set(GLFW_LIB ${PROJECT_SOURCE_DIR}/../3th/glfw/lib) 
set(GLAD_INCLUDE ${PROJECT_SOURCE_DIR}/../3th/glad/include)
set(IMAGE_INCLUDE ${PROJECT_SOURCE_DIR}/../3th/img)
set(GLM_INCLUDE ${PROJECT_SOURCE_DIR}/../3th/glm)

include_directories(${GLFW_INCLUDE} ${GLAD_INCLUDE} ${IMAGE_INCLUDE} ${GLM_INCLUDE})
link_directories(${GLFW_LIB})

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/ DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/../3th/glad/src/ DIR_SRCS)

set(TEST_MATH ${DIR_SRCS})

add_executable(${PROJECT_NAME} ${TEST_MATH})

target_link_libraries(${PROJECT_NAME} glfw3 GL)

