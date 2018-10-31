# The subdirectories to build
set(SUBDIRS player stones)

# Build subdirs
foreach(SUB ${SUBDIRS})
    add_subdirectory(${SUB})
endforeach(SUB)

# The directories that contain the classes under test
set(CLASSESUNDERTESTDIR game)

# Build all files in this directory
SET(SOURCE
    GameSavingTest.cpp
    GameLoadingTest.cpp
)

SET(HEADERS
    GameSavingTest.h
    GameLoadingTest.h
)

foreach(SUB ${CLASSESUNDERTESTDIR})
    include_directories(${CMAKE_SOURCE_DIR}/main/${SUB})
    link_directories(${CMAKE_BINARY_DIR}/main/${SUB})
endforeach(SUB)

source_group("Header Files" FILES ${HEADERS})

list(LENGTH SOURCE tmp)
math(EXPR len "${tmp} - 1")

foreach(index RANGE ${len})
    list(GET SOURCE ${index} src)
    list(GET HEADERS ${index} hdr)
    get_filename_component(TEST_EXE ${src} NAME_WE)
        
    add_executable(${TEST_EXE} ${src})
    add_test(NAME ${TEST_EXE} COMMAND ${TEST_EXE} -xunitxml -o ${TEST_EXE}-result.xml)
    target_link_libraries(${TEST_EXE} Game Qt5::Test)
endforeach()