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

add_executable(GameTests ${SOURCE})
add_test(NAME GameTests COMMAND GameTests)
target_link_libraries(GameTests Game Utils gtest gtest_main)