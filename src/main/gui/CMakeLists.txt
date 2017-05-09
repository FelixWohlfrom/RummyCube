# The subdirectories to build
set(SUBDIRS animations)

# Build all files in this directory
SET(SOURCE
    MainWindow.cpp
    Tutorial.cpp
    ImagePanel.cpp
    SettingsWindow.cpp
    CreateNetworkGameWindow.cpp
    JoinNetworkGameWindow.cpp
    AboutWindow.cpp
)

SET(HEADERS
    MainWindow.h
    Tutorial.h
    ImagePanel.h
    SettingsWindow.h
    CreateNetworkGameWindow.h
    JoinNetworkGameWindow.h
    AboutWindow.h
)

# Build subdirs and include for build
foreach(SUB ${SUBDIRS})
    include_directories(${SUB})
    link_directories(${PROJECT_BINARY_DIR}/${SUB})
    add_subdirectory(${SUB})
endforeach(SUB)

# build time in UTC ISO 8601
# Will create a new header "Timestamp.h" which will define timestamp in _TIMESTAMP_
file(WRITE ${CMAKE_BINARY_DIR}/timestamp.cmake "STRING(TIMESTAMP TIME UTC)\n")
file(APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(WRITE Timestamp.h \"#ifndef TIMESTAMP_H\\n\")\n")
file(APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(APPEND Timestamp.h \"#define TIMESTAMP_H\\n\\n\")\n")
file(APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(APPEND Timestamp.h \"#define _TIMESTAMP_ \\\"\${TIME}\\\"\\n\\n\")\n")
file(APPEND ${CMAKE_BINARY_DIR}/timestamp.cmake "FILE(APPEND Timestamp.h \"#endif // TIMESTAMP_H\\n\")\n")
add_custom_target (
    Timestamp
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/timestamp.cmake
    ADD_DEPENDENCIES ${CMAKE_BINARY_DIR}/timestamp.cmake)

source_group("Header Files" FILES ${HEADERS} Timestamp.h)
add_library(Gui ${SOURCE} ${HEADERS})
add_dependencies(Gui Timestamp)
target_link_libraries(Gui Game Network Animations Qt5::Widgets)
