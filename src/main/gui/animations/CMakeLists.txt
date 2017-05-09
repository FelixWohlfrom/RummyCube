# Build all files in this directory
SET(SOURCE
    Animation.cpp
    DisplayJoker.cpp
    ElementBlinker.cpp
    SameColorIncrementingNumber.cpp
    SameColorIncrementingNumberJoker.cpp
    SameNumberDifferentColor.cpp
    SameNumberDifferentColorJoker.cpp
    StoneMover.cpp
)

SET(HEADERS
    Animation.h
    DisplayJoker.h
    ElementBlinker.h
    SameColorIncrementingNumber.h
    SameColorIncrementingNumberJoker.cpp
    SameNumberDifferentColor.h
    SameNumberDifferentColorJoker.h
    StoneMover.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Animations ${SOURCE} ${HEADERS})
target_link_libraries(Animations Qt5::Widgets)