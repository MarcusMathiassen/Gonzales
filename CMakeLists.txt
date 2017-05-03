cmake_minimum_required(VERSION 3.6.2)
project(app)

# Compiler flags
set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-function -Wno-unused-parameter" )

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Load headers
include_directories("./src/")

# Load sources
file(GLOB SOURCES "./src/*.cpp")


if(WIN32)
	add_executable(${PROJECT_NAME} ${SOURCES})
	target_link_libraries(${PROJECT_NAME} " -opengl32 -lglew32s -lglew3dll -lkernel32")
elseif(APPLE)
	 # OPENGL
	find_package(OpenGL REQUIRED)
	if (OPENGL_FOUND)
	  include_directories(${OPENGL_INCLUDE_DIRS})
	  link_libraries(${OPENGL_LIBRARIES})
	endif()

	 # OPENGL
	find_package(boost REQUIRED)
	if (BOOST_FOUND)
	  include_directories(${BOOST_INCLUDE_DIRS})
	  link_libraries(${BOOST_LIBRARIES})
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
endif()
