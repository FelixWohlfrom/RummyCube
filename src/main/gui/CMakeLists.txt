# Build all files in this directory
SET(SOURCE
	MainWindow.cpp
)

SET(HEADERS
	MainWindow.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Gui ${SOURCE} ${HEADERS})
target_link_libraries(Gui Game Qt5::Widgets)