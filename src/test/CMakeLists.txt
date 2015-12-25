# The subdirectories to build
set(SUBDIRS game)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SUB})
	link_directories(${CMAKE_CURRENT_BINARY_DIR}/${SUB})
	add_subdirectory(${SUB})
endforeach(SUB)

#add_executable(RummyCubeTests Main.cpp)
#add_test(RummyCubeTests RummyCubeTests)
#target_link_libraries(RummyCubeTests Qt5::Test)