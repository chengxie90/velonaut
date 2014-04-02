file(GLOB_RECURSE GAME_SRCS *.cpp)

add_executable(${PROJECT_NAME} ${GAME_SRCS})

target_link_libraries(${PROJECT_NAME} engine)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/game)