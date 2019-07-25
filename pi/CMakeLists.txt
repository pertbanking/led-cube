cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

project(LEDCube)


####################################
## USB client
####################################

add_subdirectory(serial)

add_executable(
	animate
		src/main.cpp
		src/LEDCube.cpp
		src/LEDCubeUSBStreamer.cpp
		src/Animation.cpp
		src/animations/Rain.cpp
		src/animations/PlaneWop.cpp
		src/animations/CubeWop_I.cpp
		src/animations/Waves_I.cpp
		src/animations/FlashFade.cpp
		src/animations/Suspension.cpp
		src/animations/Waves_II.cpp
		src/animations/Wiper.cpp
		src/animations/CubeConstructor.cpp
)

#target_compile_options(animate PUBLIC -Wall -Wextra -Werror)

target_include_directories(animate PUBLIC "${PROJECT_SOURCE_DIR}/serial/include")

target_link_libraries(animate serial)

set_target_properties(animate
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)



#####################################
## 3D visualizer
#####################################

add_executable(
	visualizer
		src/3DViewer.cpp
)

find_package(OpenGL)
find_package(GLUT)

if (NOT OpenGL_FOUND)
	message(WARNING "OpenGL was not found. You cannot run 'make visualizer', but you can still run 'make' and create the basic LED cube USB client.")
endif()

if (NOT GLUT_FOUND)
	message(WARNING "GLUT was not found. You cannot run 'make visualizer', but you can still run 'make' and create the basic LED cube USB client.")
endif()

target_include_directories(visualizer PUBLIC ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_link_libraries(visualizer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

set_target_properties(visualizer
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)
