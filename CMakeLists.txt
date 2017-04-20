cmake_minimum_required(VERSION 3.6.2)
project(app)

# Compiler flags
set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wno-unused-function")

if(WIN32)
    #Windows specific code
elseif(APPLE)
    #OSX specific code
endif()

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Load headers
include_directories("./src/")

# Load sources
file(GLOB SOURCES "./src/*.cpp")

# GLEW
# find_package(GLEW REQUIRED)
# if (GLEW_FOUND)
#   include_directories(${GLEW_INCLUDE_DIRS})
#   link_libraries(${GLEW_LIBRARIES})
# endif()

# #GLFW3
# find_package(GLFW3 3.2 REQUIRED)
# if (GLFW3_FOUND)
#   include_directories(${GLFW3_INCLUDE_DIRS})
#   link_libraries(${GLFW3_LIBRARIES})
# endif()

# OPENGL
find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
  include_directories(${OPENGL_INCLUDE_DIRS})
  link_libraries(${OPENGL_LIBRARIES})
endif()

# GLEW
if (!${GLEW_FOUND})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GL/include)
endif()

# GLFW
if (!${GLFW3_FOUND})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/include)
endif()


add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glew ${CMAKE_CURRENT_SOURCE_DIR}/lib/GL/libGLEW.a)
target_link_libraries(${PROJECT_NAME} glfw ${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/libglfw3.a)
