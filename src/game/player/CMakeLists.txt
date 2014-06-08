# Build all files in this directory
SET(SOURCE
	Player.cpp
	OpponentPlayer.cpp
	AIPlayer.cpp
	NetworkPlayer.cpp
	NetworkPlayerServer.cpp
	NetworkPlayerClient.cpp
	HumanPlayer.cpp
	HumanPlayerServer.cpp
	HumanPlayerClient.cpp
)

SET(HEADERS
	Player.h
	OpponentPlayer.h
	AIPlayer.h
	NetworkPlayer.h
	NetworkPlayerServer.h
	NetworkPlayerClient.h
	HumanPlayer.h
	HumanPlayerServer.h
	HumanPlayerClient.h
)

source_group("Header Files" FILES ${HEADERS})
add_library(Player ${SOURCE} ${HEADERS})