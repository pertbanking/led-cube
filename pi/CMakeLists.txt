cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

project(animate)

add_subdirectory(serial)

add_executable(
	animate
		src/main.cpp
		src/LEDCube.cpp
		src/Animation.cpp
		src/animations/Rain.cpp
		src/animations/PlaneWop.cpp
		src/animations/CubeWop_I.cpp
		src/animations/Waves_I.cpp
		src/animations/FlashFade.cpp
		src/animations/Suspension.cpp
		src/animations/Waves_II.cpp
)

target_include_directories(animate PUBLIC "${PROJECT_SOURCE_DIR}/serial/include")

target_link_libraries(animate serial)

set_target_properties(animate
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)
