# Build all files in this directory
SET(SOURCE
	ImagePanel.cpp
	TutorialWindow.cpp
	PropertiesWindow.cpp
	CreateNetworkGameWindow.cpp
	JoinNetworkGameWindow.cpp
	AboutWindow.cpp
	MainWindow.cpp
)

SET(HEADERS
	ImagePanel.h
	TutorialWindow.h
	PropertiesWindow.h
	CreateNetworkGameWindow.h
	JoinNetworkGameWindow.h
	AboutWindow.h
	MainWindow.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Gui ${SOURCE} ${HEADERS})
target_link_libraries(Gui Game I18n Network ${wxWidgets_LIBRARIES})