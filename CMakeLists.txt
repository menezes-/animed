cmake_minimum_required(VERSION 3.5)
project(animation_editor)

include(CheckCXXCompilerFlag)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Winvalid-offsetof -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -s -O2 -DNDEBUG")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0")

CHECK_CXX_COMPILER_FLAG("-fsanitize=address" HAS_ASAN_FLAG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin/}")

if (HAS_ASAN_FLAG)
    message(STATUS "Enabling address sanitizer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
endif ()




include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/)


#======================OPENGL======================
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if (NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif ()
#======================OPENGL======================


#======================GLEW=====================
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLEW/include)

add_library(GLEW STATIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/GLEW/src/glew.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/GLEW/include/GL/glew.h)

SET_TARGET_PROPERTIES(GLEW PROPERTIES
        COMPILE_FLAGS "-DGLEW_STATIC"
        )

target_link_libraries(GLEW ${OPENGL_LIBRARIES})
#======================GLEW=====================

#======================GLFW=====================
set(GLFW_BUILD_EXAMPLES NO)
set(GLFW_BUILD_TESTS NO)
set(GLFW_BUILD_DOCS NO)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/include)
#======================GLFW=====================

#======================SOIL2=====================
# fork da biblioteca SOIL original. Projeto: https://bitbucket.org/SpartanJ/soil2/overview
add_library(SOIL2 STATIC IMPORTED)
SET_TARGET_PROPERTIES(SOIL2 PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsoil2.a)
#======================SOIL2=====================


#======================ASSIMP=====================
set(ASSIMP_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp)
add_library(assimp SHARED IMPORTED)

set_property(TARGET assimp PROPERTY IMPORTED_LOCATION ${ASSIMP_ROOT}/bin/libassimp.dll)

set_property(TARGET assimp PROPERTY IMPORTED_IMPLIB ${ASSIMP_ROOT}/lib/libassimp.dll.a)
include_directories(${ASSIMP_ROOT}/include)

#======================ASSIMP=====================

set(SOURCE_FILES main.cpp src/Camera.cpp
        include/Camera.hpp
        include/Shader.hpp
        src/Shader.cpp
        include/TextureLoader.hpp
        src/TextureLoader.cpp
        src/Mesh.cpp
        include/Mesh.hpp
        src/Model.cpp
        include/Model.hpp include/utils.hpp include/base64.hpp src/base64.cpp)
add_executable(animation_editor ${SOURCE_FILES})
target_link_libraries(animation_editor SOIL2 GLEW glfw ${GLFW_LIBRARIES} assimp)

set_target_properties(animation_editor PROPERTIES OUTPUT_NAME "AnimEd-${CMAKE_BUILD_TYPE}")