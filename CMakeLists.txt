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


set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/)


include_directories(${LIB_DIR})

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
include_directories(${LIB_DIR}/GLEW/include)

add_library(GLEW STATIC ${LIB_DIR}/GLEW/src/glew.c
        ${LIB_DIR}/GLEW/include/GL/glew.h)

SET_TARGET_PROPERTIES(GLEW PROPERTIES
        COMPILE_FLAGS "-DGLEW_STATIC"
        )

target_link_libraries(GLEW ${OPENGL_LIBRARIES})
#======================GLEW=====================

#======================GLFW=====================
set(GLFW_BUILD_EXAMPLES NO)
set(GLFW_BUILD_TESTS NO)
set(GLFW_BUILD_DOCS NO)

add_subdirectory(${LIB_DIR}/GLFW/)
include_directories(${LIB_DIR}/GLFW/include)
#======================GLFW=====================

#======================FMTLIB=====================
add_library(fmt STATIC ${LIB_DIR}/fmt/format.cc ${LIB_DIR}fmt/format.h)
#======================FMTLIB=====================

#======================SOIL2=====================
# fork da biblioteca SOIL original. Projeto: https://bitbucket.org/SpartanJ/soil2/overview
add_library(SOIL2 STATIC IMPORTED)
SET_TARGET_PROPERTIES(SOIL2 PROPERTIES IMPORTED_LOCATION ${LIB_DIR}libsoil2.a)
#======================SOIL2=====================


#======================ASSIMP=====================
set(ASSIMP_ROOT ${LIB_DIR}/assimp)
add_library(assimp SHARED IMPORTED)

set_property(TARGET assimp PROPERTY IMPORTED_LOCATION ${ASSIMP_ROOT}/bin/libassimp.dll)

set_property(TARGET assimp PROPERTY IMPORTED_IMPLIB ${ASSIMP_ROOT}/lib/libassimp.dll.a)
include_directories(${ASSIMP_ROOT}/include)
#======================ASSIMP=====================


#======================IMGUI=====================
set(IMGUI_ROOT ${LIB_DIR}/imgui)
add_library(imgui
        STATIC
        ${IMGUI_ROOT}/imgui.cpp
        ${IMGUI_ROOT}/imconfig.h
        ${IMGUI_ROOT}/imgui.h
        ${IMGUI_ROOT}/imgui_draw.cpp
        ${IMGUI_ROOT}/imgui_demo.cpp
        ${IMGUI_ROOT}/imgui_internal.h
        ${IMGUI_ROOT}/stb_rect_pack.h
        ${IMGUI_ROOT}/stb_textedit.h
        ${IMGUI_ROOT}/stb_truetype.h)
include_directories(${IMGUI_ROOT})
#======================IMGUI=====================

#======================INI=====================
add_library(INIH STATIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/ini.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/ini.h)
#======================INI=====================

#======================PTHREADS=====================
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)
#======================PTHREADS=====================


set(SOURCE_FILES main.cpp src/OpenGL/Camera.cpp
        include/OpenGL/Camera.hpp
        include/OpenGL/Shader.hpp
        src/OpenGL/Shader.cpp
        include/Loaders/TextureLoader.hpp
        src/Loaders/TextureLoader.cpp
        src/Objects/Mesh.cpp
        include/Objects/Mesh.hpp
        src/Objects/Model.cpp
        include/Objects/Model.hpp
        include/Constants.hpp
        include/Utils/base64.hpp
        src/Utils/base64.cpp
        src/Light/PointLight.cpp
        include/Light/PointLight.hpp
        ${LIB_DIR}/imgui_impl_glfw_gl3.h
        ${LIB_DIR}/imgui_impl_glfw_gl3.cpp
        src/Config.cpp
        include/Config.hpp
        include/Utils/Utils.hpp
        src/Utils/Utils.cpp
        src/GUI.cpp
        include/GUI.hpp
        src/Scene.cpp
        include/Scene.hpp
        src/Loaders/ShaderLoader.cpp
        include/Loaders/ShaderLoader.hpp
        src/Transform.cpp
        include/Transform.hpp
        src/Light/Light.cpp
        include/Light/Light.hpp
        src/Light/SpotLight.cpp
        include/Light/SpotLight.hpp
        src/Objects/Floor.cpp
        include/Objects/Floor.hpp)
add_executable(animation_editor ${SOURCE_FILES})

#======================SHADERS=====================
#add_custom_command(TARGET animation_editor POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:animation_editor>/shaders)
#======================SHADERS=====================

target_link_libraries(animation_editor SOIL2 GLEW glfw ${GLFW_LIBRARIES} assimp fmt imgui INIH winmm.lib imm32.lib)

set_target_properties(animation_editor PROPERTIES OUTPUT_NAME "AnimEd-${CMAKE_BUILD_TYPE}")
