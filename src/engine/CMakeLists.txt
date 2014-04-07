set(BULLET_ROOT ${LIB_DIR})
set(ENV{OGRE_HOME} ${LIB_DIR})
set(ENV{LUA_DIR} ${LIB_DIR})
set(ENV{RAKNET_ROOT} ${LIB_DIR})

find_package(Bullet REQUIRED)
find_package(OGRE REQUIRED)
find_package(OIS REQUIRED)
find_package(Lua REQUIRED)
find_package(RakNet REQUIRED)

include_directories(${BULLET_INCLUDE_DIRS} 
                    ${OGRE_INCLUDE_DIRS}
                    ${LUA_INCLUDE_DIR}
                    ${RakNet_INCLUDE_DIRS}
                    ${OIS_INCLUDE_DIRS}
                    ${OGRE_Overlay_INCLUDE_DIRS})

file(GLOB_RECURSE ENGINE_SRCS *.cpp)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${ENGINE_SRCS})

# set_target_properties(${PROJECT_NAME}
#     PROPERTIES LINK_FLAGS "-framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo -framework ApplicationServices")
# set(${CMAKE_CXX_FLAGS} "-framework IOKit -framework Cocoa -framework Carbon -framework CoreVideo -framework ApplicationServices")

target_link_libraries(${PROJECT_NAME}   ${BULLET_LIBRARIES} 
                                        ${OGRE_LIBRARIES}
                                        ${LUA_LIBRARIES}
                                        ${RakNet_LIBRARIES}
                                        ${OIS_LIBRARIES}
                                        ${OGRE_Overlay_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/game/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})


