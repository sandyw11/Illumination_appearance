# Project name
project(hw1)

# Minimum Cmake Version
cmake_minimum_required(VERSION 2.6)

# Set Module path.
set(CMAKE_MODULE_PATH ./cmake)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${hw1_SOURCE_DIR}/include)

# Find necessary libraries
###############################################
# 1) Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_libraries(${Boost_LIBRARIES})

# 2) CLPACK
find_package(CLapack REQUIRED)
include_directories(${CLAPACK_INCLUDE_DIRS})
link_libraries(${CLAPACK_LIBRARIES})

# 3) OpenEXR
find_package(OpenEXR)
if(OPENEXR_FOUND)
	include_directories(${OPENEXR_INCLUDE_DIRS})
	link_libraries(${OPENEXR_LIBRARIES})
	add_definitions(-DINCLUDE_OPENEXR)
endif(OPENEXR_FOUND)

# 4) PNG
find_package(PNG)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
	link_libraries(${PNG_LIBRARIES})
	add_definitions(-DINCLUDE_PNG)
endif(PNG_FOUND)

# 5) JPEG
find_package(JPEG)
if(JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIRS})
	link_libraries(${JPEG_LIBRARIES})
	add_definitions(-DINCLUDE_JPEG)
endif(JPEG_FOUND)

# 5) TIFF
find_package(TIFF)
if(TIFF_FOUND)
	include_directories(${TIFF_INCLUDE_DIR})
	link_libraries(${TIFF_LIBRARIES})
	add_definitions(-DINCLUDE_TIFF)
endif(TIFF_FOUND)
###############################################

# set include dirs
include_directories(${hw1_SOURCE_DIR}/include)

# get CPP files
file(GLOB SRC src/*cpp)

# get executables
file(GLOB EXECLIST bin/*cpp)

foreach(EXEC ${EXECLIST})
  GET_FILENAME_COMPONENT(BASE ${EXEC} NAME_WE)
  message("-- Adding compile target: ${BASE}")
  add_executable(${BASE} ${SRC} ${EXEC})
endforeach(EXEC)
