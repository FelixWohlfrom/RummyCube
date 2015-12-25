# The subdirectories to build
set(SUBDIRS stones)

# Build all files in this directory
#SET(SOURCE
#	CountdownTimer.cpp
#	RummyCube.cpp
#	RummyCubeNetwork.cpp
#)

#SET(HEADERS
#	CountdownTimer.h
#	RummyCube.h
#	RummyCubeNetwork.h
#)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SUB})
	link_directories(${CMAKE_CURRENT_BINARY_DIR}/${SUB})
	add_subdirectory(${SUB})
endforeach(SUB)

#add_library(GameTests ${SOURCE} ${HEADERS})
#target_link_libraries(GameTests StoneTests)