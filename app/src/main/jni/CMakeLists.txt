# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 4.1.2)

project("Sky")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

include_directories(imgui)
include_directories(imgui/backends)
include_directories(Tool)
include_directories(.)

file(GLOB_RECURSE native_srcs ./*.cpp)
add_library( Sky

        # Sets the library as a shared library.
        SHARED
        ${native_srcs}
        )


find_library(
        log-lib
        log)
        
target_compile_options(Sky PRIVATE
    -w
)

target_link_libraries( Sky
        android
        EGL
        GLESv3
        log
        )