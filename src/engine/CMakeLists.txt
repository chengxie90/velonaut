set(BULLET_ROOT ${LIB_DIR})
set(ENV{OGRE_HOME} ${LIB_DIR})
set(ENV{LUA_DIR} ${LIB_DIR})
set(ENV{RAKNET_ROOT} ${LIB_DIR})
set(ENV{SDL2DIR} ${LIB_DIR})
set(ENV{ROCKET_DIR} ${LIB_DIR})
set(SDL2_BUILDING_LIBRARY 1)

find_package(Bullet REQUIRED)
find_package(OGRE REQUIRED)
find_package(Lua REQUIRED)
find_package(RakNet REQUIRED)
find_package(SDL2 REQUIRED)
find_package(LibRocket REQUIRED)

include_directories(${BULLET_INCLUDE_DIRS} 
                    ${OGRE_INCLUDE_DIRS}
                    ${LUA_INCLUDE_DIR}
                    ${RakNet_INCLUDE_DIRS}
                    ${SDL2_INCLUDE_DIR}
                    ${LIBROCKET_INCLUDE_DIRS})

file(GLOB_RECURSE ENGINE_SRCS *.cpp *.h)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME} ${ENGINE_SRCS})

if(APPLE)
    set_source_files_properties(graphics.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
endif()

target_link_libraries(${PROJECT_NAME}   ${BULLET_LIBRARIES} 
                                        ${OGRE_LIBRARIES}
                                        ${LUA_LIBRARIES}
                                        ${RakNet_LIBRARIES}
                                        ${SDL2_LIBRARY}
                                        ${LIBROCKET_LIBRARY_CONTROLS}
                                        ${LIBROCKET_LIBRARY_CORE}
                                        ${LIBROCKET_LIBRARY_DEBUGGER})

message(${LIBROCKET_LIBRARIES})

cotire(${PROJECT_NAME})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/game/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})


