# The subdirectories to build
set(SUBDIRS stones player)

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

source_group("Header Files" FILES ${HEADERS} Settings.h)
add_library(Game ${SOURCE} ${HEADERS})
add_library(Settings Settings.cpp Settings.h)
target_link_libraries(Settings Qt5::Core)

#target_link_libraries(Game Settings Player Stones Network)
target_link_libraries(Game Settings Player)