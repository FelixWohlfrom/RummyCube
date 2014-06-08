# Build all files in this directory
SET(SOURCE
	StoneManagerTests.cpp
)

add_executable(StoneTests ${SOURCE} ${HEADERS})
add_test(NAME StoneTests COMMAND StoneTests)
target_link_libraries(StoneTests Stones Utils gtest gtest_main)