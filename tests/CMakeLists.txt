include_directories(${PROJECT_SOURCE_DIR}/libs/gtest-1.6.0/include)

add_executable(RummyCubeTest Main.cpp)
target_link_libraries(RummyCubeTest gtest gtest_main)