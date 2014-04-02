set(BULLET_ROOT ${LIB_DIR}/bullet)

find_package(Bullet REQUIRED)

include_directories(${BULLET_INCLUDE_DIRS})

file(GLOB_RECURSE ENGINE_SRCS *.cpp)

add_library(engine STATIC ${ENGINE_SRCS})

target_link_libraries(engine ${BULLET_LIBRARIES})

