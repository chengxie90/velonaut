set(BULLET_ROOT ${LIB_DIR})
set(ENV{OGRE_HOME} ${LIB_DIR})

find_package(Bullet REQUIRED)
find_package(OGRE REQUIRED)

include_directories(${BULLET_INCLUDE_DIRS} 
                    ${OGRE_INCLUDE_DIRS})

file(GLOB_RECURSE ENGINE_SRCS *.cpp)

add_library(engine STATIC ${ENGINE_SRCS})

target_link_libraries(engine ${BULLET_LIBRARIES} 
                             ${OGRE_LIBRARIES})

