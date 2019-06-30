cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

project(animate)

add_subdirectory(serial)

add_executable(
	animate
		animation_generator/main.cpp
		animation_generator/LEDCube.cpp
		animation_generator/Animation.cpp
		animation_generator/animations/Rain.cpp
		animation_generator/animations/PlaneWop.cpp
)

target_include_directories(animate PUBLIC "${PROJECT_SOURCE_DIR}/serial/include")

target_link_libraries(animate serial "${CMAKE_SOURCE_DIR}/build/serial/libserial.a" pthread rt)

set_target_properties(animate
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)
