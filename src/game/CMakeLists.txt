add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} engine)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/game)