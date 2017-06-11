# The subdirectories to build
set(SUBDIRS game gui i18n network)

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

# Image resource files
set(CMAKE_AUTORCC ON)
set(RSRC gui/resources/RummyCube.qrc)
if(WIN32)
    set(RSRC ${RSRC} gui/resources/msw/RummyCube.rc)
endif(WIN32)

# Generate the main program
source_group("Header Files" FILES Version.h)
add_executable(RummyCube ${GUI_TYPE} ${RSRC} Main.cpp)
target_link_libraries(RummyCube Gui)
