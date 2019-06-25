cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_STANDARD 14)

project(animate)

add_subdirectory(serial)

add_executable(animate
	animation_generator/main.cpp
	animation_generator/LEDCube.cpp
	animation_generator/Animation.cpp
)
target_link_libraries(animate serial)

install(TARGETS animate DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
