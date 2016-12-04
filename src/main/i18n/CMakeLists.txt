# Build the internationalization
# The language files
SET(LANG
    RummyCube_de.ts
)

SET(SOURCE
    Internationalization.cpp
)

SET(HEADERS
    Internationalization.h
)

source_group("Header Files" FILES ${HEADERS})

find_package(Qt5LinguistTools)
qt5_add_translation(TRANSLATIONS ${LANG})
add_library(I18n ${TRANSLATIONS} ${SOURCE} ${HEADER})
target_link_libraries(I18n Qt5::Core Qt5::Widgets)