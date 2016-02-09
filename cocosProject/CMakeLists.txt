cmake_minimum_required(VERSION 2.8)

set(APP_NAME FoxyBird)
project (${APP_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(Cocos2dx_DIR "${CMAKE_SOURCE_DIR}/../../cocos2d-x")

include_directories(${CMAKE_SOURCE_DIR}/Game)
include_directories(${CMAKE_SOURCE_DIR}/external)
link_directories(${Cocos2dx_DIR}/lib/)

#TODO: remove it cuz it sohuld be taken from cocos2d-x project
message(STATUS ${Cocos2dx_DIR}/external/)
include_directories(
    ${Cocos2dx_DIR}
    ${Cocos2dx_DIR}/external/
    ${Cocos2dx_DIR}/external/glfw3/include/mac/
    ${Cocos2dx_DIR}/cocos
    ${Cocos2dx_DIR}/cocos/audio/include
    ${Cocos2dx_DIR}/cocos/platform
    ${Cocos2dx_DIR}/cocos/platform/mac
    ${Cocos2dx_DIR}/cocos/editor-support
    ${Cocos2dx_DIR}/extensions
    ${Cocos2dx_DIR}/external
    ${Cocos2dx_DIR}/external/edtaa3func
    ${Cocos2dx_DIR}/external/chipmunk/include/chipmunk
    ${Cocos2dx_DIR}/external/jpeg/include/mac
    ${Cocos2dx_DIR}/external/tiff/include/mac
    ${Cocos2dx_DIR}/external/webp/include/mac
    ${Cocos2dx_DIR}/external/tinyxml2
    ${Cocos2dx_DIR}/external/unzip
    ${Cocos2dx_DIR}/external/freetype2/include/mac
    ${Cocos2dx_DIR}/external/websockets/include/mac
    ${Cocos2dx_DIR}/external/spidermonkey/include/liniosux
    ${Cocos2dx_DIR}/external/xxhash
)
link_directories(
    ${Cocos2dx_DIR}/external/jpeg/prebuilt/mac
    ${Cocos2dx_DIR}/external/chipmunk/prebuilt/mac
    ${Cocos2dx_DIR}/external/tiff/prebuilt/mac
    ${Cocos2dx_DIR}/external/freetype2/prebuilt/mac
    ${Cocos2dx_DIR}/external/webp/prebuilt/mac
)

#end

#TODO: use external project
#find_package(Cocos2dx REQUIRED)

add_subdirectory(Game)
add_executable(${APP_NAME} 
    proj.ios_mac/mac/main.cpp
    proj.ios_mac/mac/Managers/AdvertisingManager.cpp
    proj.ios_mac/mac/Managers/DataManager.cpp
    proj.ios_mac/mac/Managers/EconomicsManager.cpp
)

#TODO remove it
target_link_libraries(${APP_NAME} GLFW3 unzip png jpeg z chipmunk tinyxml2 tiff freetype webp xxhash sqlite3)
#end

target_link_libraries(${APP_NAME} cocos2d Game)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set_target_properties(
    ${APP_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}"
)

include(../../cocos2d-x/cmake/Modules/CocosBuildHelpers.cmake)
pre_build(
    ${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
