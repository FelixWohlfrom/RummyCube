# Build all files in this directory
SET(SOURCE
	DummyApp.cpp
	utils.cpp
)

SET(HEADERS
	DummyApp.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Utils ${SOURCE} ${HEADERS})
target_link_libraries(Utils ${wxWidgets_LIBRARIES} gtest gtest_main)