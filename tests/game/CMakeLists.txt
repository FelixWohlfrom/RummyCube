# The subdirectories to build
set(SUBDIRS stones)

# Build all files in this directory
SET(SOURCE
	NetworkGameTests.cpp
)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SUB})
	link_directories(${CMAKE_CURRENT_BINARY_DIR}/${SUB})
	add_subdirectory(${SUB})
endforeach(SUB)

if(WIN32)
	include_directories(../../src/gui/resources/msw)
	set(RSRC ../../src/gui/resources/msw/RummyCube.rc)
endif(WIN32)

add_executable(GameTests ${RSRC} ${SOURCE})
add_test(NAME GameTests COMMAND GameTests)
target_link_libraries(GameTests Game Utils gtest gtest_main)