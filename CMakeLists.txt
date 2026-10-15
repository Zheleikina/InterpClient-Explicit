cmake_minimum_required(VERSION 3.20)
project(InterpClientExplicit LANGUAGES CXX)

add_executable(ExplicitApp main.cpp)
target_compile_features(ExplicitApp PRIVATE cxx_std_20)

add_custom_command(TARGET ExplicitApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/InterpolationLib.dll"
        "$<TARGET_FILE_DIR:ExplicitApp>")
