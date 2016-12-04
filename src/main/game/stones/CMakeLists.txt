# Build all files in this directory
SET(SOURCE
    Gamestone.cpp
    Joker.cpp
    ParentChangedEvent.cpp
    StoneManager.cpp
)

SET(HEADERS
    Gamestone.h
    Joker.h
    ParentChangedEvent.h
    StoneManager.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Stones ${SOURCE} ${HEADERS})
target_link_libraries(Stones Settings Qt5::Widgets)