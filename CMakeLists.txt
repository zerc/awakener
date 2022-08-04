cmake_minimum_required(VERSION 3.22)
project(awakener C)

set(CMAKE_C_STANDARD 17)

add_executable(awakener src/server.h src/server.c src/main.c)
add_subdirectory(vendor/check)

add_executable(awakener_check tests/check_request.c src/server.c)

set(STATIC_LIBS check)
message("Using static libs: ${STATIC_LIBS}")
target_link_libraries(awakener_check ${STATIC_LIBS})

add_custom_target(copy-templates ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/public ${CMAKE_BINARY_DIR}/public
)

if (CMAKE_BUILD_TYPE EQUAL ReleaseAarch)
    add_custom_command(
            TARGET awakener
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/awakener /var/www/awakener/awakener
    )
endif ()

enable_testing()
add_test(NAME awakener_check COMMAND awakener_check)