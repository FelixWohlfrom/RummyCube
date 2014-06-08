# Build all files in this directory
SET(SOURCE
	StoneManagerTests.cpp
)

if(WIN32)
	include_directories(../../../src/gui/resources/msw)
	set(RSRC ../../../src/gui/resources/msw/RummyCube.rc)
endif(WIN32)

add_executable(StoneTests ${RSRC} ${SOURCE} ${HEADERS})
add_test(NAME StoneTests COMMAND StoneTests)
target_link_libraries(StoneTests Stones Utils gtest gtest_main)