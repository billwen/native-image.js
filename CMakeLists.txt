cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)
set (CMAKE_CXX_STANDARD 20)

project (js-lib-vips)

# the `pkg_check_modules` function is created with this call
find_package(PkgConfig REQUIRED) 
pkg_check_modules(VIPS REQUIRED vips-cpp vips glib-2.0)

add_definitions(-DNAPI_VERSION=8)

include_directories(${CMAKE_JS_INC})

file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${VIPS_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${VIPS_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${SDL2_CFLAGS_OTHER})

# Include Node-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})
