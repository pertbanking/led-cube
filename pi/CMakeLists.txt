cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

project(animate)

add_subdirectory(serial)
include_directories ("${PROJECT_SOURCE_DIR}/serial/include")

add_executable(animate
	animation_generator/main.cpp
	animation_generator/LEDCube.cpp
	animation_generator/Animation.cpp
	animation_generator/animations/Rain.cpp
	animation_generator/animations/PlaneWop.cpp
)
target_link_libraries(animate ${CMAKE_SOURCE_DIR}/build/serial/libserial.a pthread rt)

install(TARGETS animate DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})
