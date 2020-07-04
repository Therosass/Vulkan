cmake_minimum_required(VERSION 3.10)

# environment variables

set( ENV{GLFW} "C:/Temp/cmakeTest/libs/GLFW")
set( ENV{GLM} "C:/Temp/cmakeTest/libs/GLM")
set( ENV{Vulkan} "C:/VulkanSDK/1.2.141.0/Lib32/")


# set the project name
project(Tutorial)

# add the executable
add_executable(main ./src/main.cpp ./src/renderer.cpp)
target_compile_features(main PRIVATE cxx_std_17)

# includes
include_directories("libs/headers")
include_directories("headers")
include_directories("src")
include_directories("C:/VulkanSDK/1.2.141.0/Include/")

#libs
add_library(glfw STATIC IMPORTED)
add_library(glm STATIC IMPORTED)
add_library(vulkan STATIC IMPORTED)
set_property(TARGET vulkan PROPERTY IMPORTED_LOCATION "$ENV{Vulkan}/vulkan-1.lib")
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "$ENV{GLFW}/libglfw3dll.a")
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "$ENV{GLFW}/glfw3.dll")
set_property(TARGET glm PROPERTY IMPORTED_LOCATION "$ENV{GLM}/glfw3.dll")
set_property(TARGET glm PROPERTY IMPORTED_LOCATION "$ENV{GLM}/glfw3.a")

target_link_libraries(main glfw)
target_link_libraries(main vulkan)

# copy libs
add_custom_command ( TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glfw> $<TARGET_FILE_DIR:main>
)