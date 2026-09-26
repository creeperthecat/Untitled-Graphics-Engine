cmake_minimum_required(VERSION 3.20)
project(app VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SharedFileDirectory ${CMAKE_SOURCE_DIR}/src/shared CACHE PATH "Path to shared files")
set(ShaderDirectory shaders CACHE STRING "Name of shader directory")
set(TextureDirectory textures CACHE STRING "Name of texture directory")

configure_file(Config.hpp.in ${CMAKE_SOURCE_DIR}/src/headers/Config.hpp)

set(SOURCE_FILES
  src/main/app.cpp
  src/main/Globals.hpp
  src/main/programFunctions.hpp
  src/main/programFunctions.cpp)

find_package(OpenGL REQUIRED)

add_executable(app ${SOURCE_FILES})

add_library(Buffer STATIC src/classes/Buffer.cpp)
add_library(MeshBuffer STATIC src/classes/MeshBuffer.cpp)
add_library(EntityBuffer STATIC src/classes/EntityBuffer.cpp)
add_library(VertexArray STATIC src/classes/VertexArray.cpp)

add_library(Window STATIC src/classes/Window.cpp)
add_library(Shader STATIC src/classes/Shader.cpp)
add_library(Shape STATIC src/classes/Shape.cpp)
add_library(Renderer STATIC src/classes/Renderer.cpp)
add_library(Texture STATIC src/classes/Texture.cpp)
add_library(Camera STATIC src/classes/Camera.cpp)

add_library(Object STATIC src/classes/Object.cpp)
add_library(Primitive STATIC src/classes/Primitive.cpp)
add_library(FileManager STATIC src/classes/FileManager.cpp)
add_library(Stride STATIC src/classes/Stride.cpp)

add_library(Mesh STATIC src/classes/Mesh.cpp)
add_library(Model STATIC src/classes/Model.cpp)

add_library(FrameBuffer STATIC src/classes/FrameBuffer.cpp)
add_library(Cubemap STATIC src/classes/Cubemap.cpp)
add_library(UniformBuffer STATIC src/classes/UniformBuffer.cpp)
add_library(StandardShader STATIC src/classes/StandardShader.cpp)
add_library(GeometryShader STATIC src/classes/GeometryShader.cpp)

add_library(glad STATIC lib/gl.c)
add_library(SceneRenderer STATIC src/main/SceneRenderer.cpp)

file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)

include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cache)

if (UNIX AND (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/libglfw3.a OR NOT EXISTS ${CMAKE_SOURCE_DIR}/include/GLFW))
  message("glfw not found, downloading library...")
  CPMAddPackage(
    NAME glfwsource
    URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip"
    OPTIONS "GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF" "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}"
  )

  add_custom_command(
    TARGET Window
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} --build ${glfwsource_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${glfwsource_BINARY_DIR}/src/libglfw3.a ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${glfwsource_SOURCE_DIR}/include/GLFW ${CMAKE_SOURCE_DIR}/include/GLFW
  )
endif()

if (WIN32 AND (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/glfw3.lib OR NOT EXISTS ${CMAKE_SOURCE_DIR}/include/GLFW))
  message("glfw not found, downloading library...")
  CPMAddPackage(
    NAME glfwsource
    URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip"
    OPTIONS "GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF" "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}"
  )

  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    COMMAND ${CMAKE_COMMAND} -S ${glfwsource_SOURCE_DIR} -B ${glfwsource_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${glfwsource_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${glfwsource_BINARY_DIR}/src/${CMAKE_BUILD_TYPE}/glfw3.lib ${CMAKE_SOURCE_DIR}/lib/glfw3.lib
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${glfwsource_SOURCE_DIR}/include/GLFW ${CMAKE_SOURCE_DIR}/include/GLFW
  )
  add_custom_target(custom_target ALL DEPENDS ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)
endif()

add_library(libglfw STATIC IMPORTED)
if (UNIX)
  set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libglfw3.a)
elseif(WIN32)
  set_target_properties(libglfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)
endif()



if (UNIX AND (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/libassimp.so) OR NOT EXISTS ${CMAKE_SOURCE_DIR}/include/assimp)
  message("assimp not found, downloading library...")
  CPMAddPackage(
    NAME assimpsource
    URL "https://github.com/assimp/assimp/archive/refs/tags/v6.0.2.zip"
    OPTIONS "ASSIMP_BUILD_TESTS OFF"
  )

  add_custom_command(
    TARGET Mesh
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} --build ${assimpsource_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${assimpsource_BINARY_DIR}/bin ${CMAKE_SOURCE_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${assimpsource_SOURCE_DIR}/include/assimp ${CMAKE_SOURCE_DIR}/include/assimp
    WORKING_DIRECTORY ${assimpsource_SOURCE_DIR}
  )
endif()

if (WIN32 AND (NOT EXISTS ${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mtd.lib) OR NOT EXISTS ${CMAKE_SOURCE_DIR}/include/assimp)
  message("assimp not found, downloading library...")
  CPMAddPackage(
    NAME assimpsource
    URL "https://github.com/assimp/assimp/archive/refs/tags/v6.0.2.zip"
    OPTIONS "ASSIMP_BUILD_TESTS OFF"
  )

  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mtd.lib
    COMMAND ${CMAKE_COMMAND} -S ${assimpsource_SOURCE_DIR} -B ${assimpsource_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${assimpsource_BINARY_DIR} --config ${CMAKE_BUILD_TYPE}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${assimpsource_BINARY_DIR}/bin/Debug ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${assimpsource_BINARY_DIR}/lib/Debug ${CMAKE_SOURCE_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${assimpsource_SOURCE_DIR}/include/assimp ${CMAKE_SOURCE_DIR}/include/assimp
    WORKING_DIRECTORY ${assimpsource_SOURCE_DIR}
  )
  add_custom_target(custom_target2 ALL DEPENDS ${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mtd.lib)
endif()



add_library(libassimp SHARED IMPORTED)
if (UNIX)
  set_target_properties(libassimp PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libassimp.so)
elseif(WIN32)
  set_target_properties(libassimp PROPERTIES IMPORTED_IMPLIB ${CMAKE_SOURCE_DIR}/lib/assimp-vc143-mtd.lib)
endif()
add_subdirectory(lib/glm/glm)

target_include_directories(glad PUBLIC include)
target_include_directories(FileManager PUBLIC include src/main)
file(COPY ${SharedFileDirectory} DESTINATION ${CMAKE_SOURCE_DIR}/bin)

include_directories(src/headers include lib/glm src/classes)
if (WIN32)
  include_directories(include lib/glm lib/glfw-3.4)
endif()

mark_as_advanced(
  CPM_DONT_CREATE_PACKAGE_LOCK
  CPM_DONT_UPDATE_MODULE_PATH
  CPM_DOWNLOAD_ALL 
  CPM_INCLUDE_ALL_IN_PACKAGE_LOCK 
  CPM_LOCAL_PACKAGES_ONLY 
  CPM_SOURCE_CACHE 
  CPM_USE_LOCAL_PACKAGES 
  CPM_USE_NAMED_CACHE_DIRECTORIES 
  FETCHCONTENT_BASE_DIR 
  FETCHCONTENT_FULLY_DISCONNECTED 
  FETCHCONTENT_QUIET 
  FETCHCONTENT_UPDATES_DISCONNECTED 
  GLFW_BUILD_TESTS 
  GLFW_BUILD_WAYLAND 
  GLFW_BUILD_X11 
  GLFW_LIBRARY_TYPE 
  WAYLAND_SCANNER_EXECUTABLE
  X11_xcb_xkb_INCLUDE_PATH
)


target_link_libraries(Shape Stride)
target_link_libraries(Buffer Shape)
target_link_libraries(VertexArray Buffer)
target_link_libraries(Primitive VertexArray Shader Texture Object)
target_link_libraries(Renderer Camera Object Window)
target_link_libraries(Shader FileManager UniformBuffer)
target_link_libraries(Texture FileManager)
target_link_libraries(Window libglfw glad)
target_link_libraries(Mesh libassimp)
target_link_libraries(Model Mesh)
target_link_libraries(Cubemap Texture)
target_link_libraries(StandardShader Shader)
target_link_libraries(GeometryShader Shader)
target_link_libraries(SceneRenderer Primitive Model Renderer glm)

target_link_libraries(
  app
  FrameBuffer
  UniformBuffer
  SceneRenderer
  StandardShader
  GeometryShader
  VertexArray
  UniformBuffer
  MeshBuffer
)

if (UNIX)
  target_compile_options(app PRIVATE -pedantic-errors -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
  target_compile_options(glm INTERFACE -w)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "install prefix" FORCE)
endif()
install(TARGETS app DESTINATION bin)
install(DIRECTORY ${SharedFileDirectory} DESTINATION .)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME} CACHE STRING "The package name")
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_SOURCE_DIR}/_packages)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_SOURCE_DIR/_packages})
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
