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

# Comment out to generate warnings....
target_compile_options(animate PUBLIC
	# ...at all
	-Wall
	# ...in abundance
	-Wextra
	# ...as errors
	#-Werror
)

target_include_directories(animate PUBLIC "${PROJECT_SOURCE_DIR}/serial/include")

target_link_libraries(animate serial rt pthread)

set_target_properties(animate
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)



#####################################
## 3D visualizer
#####################################


find_package(OpenGL OPTIONAL_COMPONENTS)
find_package(glfw3 3.1 OPTIONAL_COMPONENTS)

if (NOT OpenGL_FOUND)
	message(WARNING "OpenGL was not found. You cannot run the visualizer, but you can still run 'make' and create the basic LED cube USB client.")
endif()

if (NOT glfw3_FOUND)
	message(WARNING "glfw3 was not found. You cannot run the visualizer, but you can still run 'make' and create the basic LED cube USB client.")
endif()

# only compile the visualizer if the user has OpenGL and glfw3 installed
if (OpenGL_FOUND AND glfw3_FOUND)

	add_executable(
		visualizer
			src/visualizer.cpp
			src/LEDCube.cpp
			src/LEDCubeVisualizer.cpp
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
	
	# Comment out to generate warnings....
	target_compile_options(visualizer PUBLIC
		# ...at all
		-Wall
		# ...in abundance
		-Wextra
		# ...as errors
		#-Werror
	)

	target_include_directories(visualizer PUBLIC ${OPENGL_INCLUDE_DIRS} ${glfw_INCLUDE_DIRS})
	target_link_libraries(visualizer rt pthread ${OPENGL_LIBRARIES} glfw)

	set_target_properties(visualizer
	    PROPERTIES
	    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
endif()
