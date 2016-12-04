# Build all files in this directory
SET(SOURCE
    MainWindow.cpp
    ImagePanel.cpp
    SettingsWindow.cpp
    CreateNetworkGameWindow.cpp
    JoinNetworkGameWindow.cpp
    AboutWindow.cpp
)

SET(HEADERS
    MainWindow.h
    ImagePanel.h
    SettingsWindow.h
    CreateNetworkGameWindow.h
    JoinNetworkGameWindow.h
    AboutWindow.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Gui ${SOURCE} ${HEADERS})
target_link_libraries(Gui Game Network Qt5::Widgets)