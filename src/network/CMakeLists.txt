# Build all files in this directory
SET(SOURCE
	Network.cpp
	NetworkEventHandlerClient.cpp
	NetworkGameClosedEvent.cpp
)

SET(HEADERS
	Network.h
	NetworkEventHandlerClient.h
	NetworkGameClosedEvent.h
	NetworkGameSettings.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Network ${SOURCE} ${HEADERS})
target_link_libraries(Network ${wxWidgets_LIBRARIES})