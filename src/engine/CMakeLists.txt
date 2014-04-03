set(BULLET_ROOT ${LIB_DIR})
set(ENV{OGRE_HOME} ${LIB_DIR})
set(ENV{LUA_DIR} ${LIB_DIR})

find_package(Bullet REQUIRED)
find_package(OGRE REQUIRED)
find_package(Lua REQUIRED)

include_directories(${BULLET_INCLUDE_DIRS} 
                    ${OGRE_INCLUDE_DIRS}
                    ${LUA_INCLUDE_DIR})

file(GLOB_RECURSE ENGINE_SRCS *.cpp)

add_library(engine STATIC ${ENGINE_SRCS})

target_link_libraries(engine ${BULLET_LIBRARIES} 
                             ${OGRE_LIBRARIES}
                             ${LUA_LIBRARIES})

