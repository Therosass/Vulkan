cmake_minimum_required(VERSION 3.10)

# environment variables

set( ENV{GLFW} "C:/Temp/cmakeTest/libs/GLFW")
set( ENV{GLM} "C:/Temp/cmakeTest/libs/GLM")
set( ENV{TINY} "C:/Temp/cmakeTest/libs/tinyobjloader")
set( ENV{Textures} "C:/Temp/cmakeTest/textures/")
set( ENV{Vulkan} "C:/VulkanSDK/1.2.141.0/Lib32/")


# set the project name
project(Tutorial)

# add the executable
add_executable(main ./src/main.cpp ./src/renderer.cpp)
target_compile_features(main PRIVATE cxx_std_17)

# includes
include_directories("libs/headers")
include_directories("libs/headers/stb")
include_directories("libs/headers/tinyobjloader")
include_directories("headers")
include_directories("src")
include_directories("C:/VulkanSDK/1.2.141.0/Include/")

#libs
add_library(glfw STATIC IMPORTED)
add_library(glm STATIC IMPORTED)
add_library(tiny STATIC IMPORTED)
add_library(vulkan STATIC IMPORTED)
set_property(TARGET vulkan PROPERTY IMPORTED_LOCATION "$ENV{Vulkan}/vulkan-1.lib")
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "$ENV{GLFW}/libglfw3dll.a")
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "$ENV{GLFW}/glfw3.dll")
set_property(TARGET glm PROPERTY IMPORTED_LOCATION "$ENV{GLM}/libglm_static.a")
set_property(TARGET glm PROPERTY IMPORTED_LOCATION "$ENV{GLM}/libglm_shared.dll.a")
set_property(TARGET tiny PROPERTY IMPORTED_LOCATION "$ENV{TINY}/libtinyobjloader.a")

target_link_libraries(main glfw)
target_link_libraries(main vulkan)

# copy libs

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/models/)


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