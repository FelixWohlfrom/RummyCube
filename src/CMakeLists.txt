# Project name and version
project(RummyCube)
set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_BETA TRUE)

# The subdirectories to build
set(SUBDIRS network game gui)

# Generate the version file
configure_file(
  "Version.h.in"
  "${PROJECT_BINARY_DIR}/Version.h"
)

# Make version file readable for project
include_directories("${PROJECT_BINARY_DIR}")

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
	include_directories(${SUB})
	link_directories(${PROJECT_BINARY_DIR}/${SUB})
	add_subdirectory(${SUB})
endforeach(SUB)

# Build the internationalization
source_group("Header Files" FILES Internationalization.h)
add_library(I18n Internationalization.cpp Internationalization.h)
target_link_libraries(I18n ${wxWidgets_LIBRARIES})
