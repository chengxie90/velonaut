set(BULLET_ROOT ${LIB_DIR}/bullet)
message(${BULLET_ROOT})
find_package(Bullet REQUIRED)

include_directories(${BULLET_INCLUDE_DIRS})

add_library(engine STATIC engine.cpp)

target_link_libraries(engine ${BULLET_LIBRARIES})

