cmake_minimum_required(VERSION 3.10)

# environment variables

set( ENV{GLFW} "C:/Temp/cmakeTest/libs/GLFW")
set( ENV{GLM} "C:/Temp/cmakeTest/libs/GLM")
set( ENV{TINY} "C:/Temp/cmakeTest/libs/tinyobjloader")
set( ENV{Textures} "C:/Temp/cmakeTest/textures/")
set( ENV{Vulkan} "C:/VulkanSDK/1.2.141.0/Lib32/")
set( ENV{BOOST} "c:/boost_1_73_0/buildgcc/boost/bin.v2/libs/")
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

# set the project name
project(Tutorial)

# add the executable
add_executable(main ./src/main.cpp ./src/renderer.cpp ./src/window_handler.cpp ./src/core.cpp ./src/module.cpp ./src/messageHandler.cpp)
target_compile_features(main PRIVATE cxx_std_17)

# includes
include_directories("libs/headers/")
include_directories("libs/headers/stb/")
include_directories("libs/headers/tinyobjloader/")
include_directories("headers/")
include_directories("src/")
include_directories("C:/VulkanSDK/1.2.148.0/Include/")
include_directories("C:/boost_1_73_0/")

#libs
add_library(glfw STATIC IMPORTED)
add_library(glm STATIC IMPORTED)
add_library(tiny STATIC IMPORTED)
add_library(vulkan STATIC IMPORTED)
add_library(boost_chrono STATIC IMPORTED)
add_library(boost_system STATIC IMPORTED)
add_library(boost_thread STATIC IMPORTED)

set_property(TARGET boost_chrono PROPERTY IMPORTED_LOCATION "$ENV{BOOST}/chrono/build/gcc-9.2.0/release/link-static/runtime-link-static/threading-multi/visibility-hidden/libboost_chrono-mgw9-mt-s-x32-1_73.a")
set_property(TARGET boost_thread PROPERTY IMPORTED_LOCATION "$ENV{BOOST}/thread/build/gcc-9.2.0/release/link-static/runtime-link-static/threadapi-win32/threading-multi/visibility-hidden/libboost_thread-mgw9-mt-s-x32-1_73.a")
set_property(TARGET boost_system PROPERTY IMPORTED_LOCATION "$ENV{BOOST}/system/build/gcc-9.2.0/release/link-static/runtime-link-static/threading-multi/visibility-hidden/libboost_system-mgw9-mt-s-x32-1_73.a")
set_property(TARGET vulkan PROPERTY IMPORTED_LOCATION "$ENV{Vulkan}/vulkan-1.lib")
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "$ENV{GLFW}/libglfw3dll.a")
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "$ENV{GLFW}/glfw3.dll")
set_property(TARGET glm PROPERTY IMPORTED_LOCATION "$ENV{GLM}/libglm_static.a")
set_property(TARGET glm PROPERTY IMPORTED_LOCATION "$ENV{GLM}/libglm_shared.dll.a")
set_property(TARGET tiny PROPERTY IMPORTED_LOCATION "$ENV{TINY}/libtinyobjloader.a")

target_link_libraries(main glfw)
target_link_libraries(main boost_chrono)
target_link_libraries(main boost_system)
target_link_libraries(main boost_thread)
target_link_libraries(main vulkan)

# copy libs and files

#models and textures
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/models/)

#libs
add_custom_command ( TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:main>
)

add_custom_command ( TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glm> $<TARGET_FILE_DIR:main>
)

add_custom_command ( TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:tiny> $<TARGET_FILE_DIR:main>
)

#config files
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/config/)