# The subdirectories to build
set(SUBDIRS game utils)

# Include the test library
include_directories(${gtest_SOURCE_DIR}/include)

# Build all files in this directory
SET(SOURCE
	Main.cpp
)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
	include_directories(${SUB})
	link_directories(${PROJECT_BINARY_DIR}/${SUB})
	add_subdirectory(${SUB})
endforeach(SUB)

add_executable(MainTest ${SOURCE})
add_test(NAME MainTest COMMAND MainTest)
target_link_libraries(MainTest gtest gtest_main)