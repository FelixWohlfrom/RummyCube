find_package(Qt5Test REQUIRED)

# The subdirectories to build
set(SUBDIRS game network)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${SUB})
    link_directories(${CMAKE_CURRENT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)

# Copies resource files to target directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})