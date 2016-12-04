# Build all files in this directory
SET(SOURCE
    Network.cpp
)

SET(HEADERS
    Network.h
    NetworkGameSettings.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Network ${SOURCE} ${HEADERS})
target_link_libraries(Network Game Qt5::Widgets Qt5::Network)