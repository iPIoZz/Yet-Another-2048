cmake_minimum_required(VERSION 3.23)
project(SDLProject)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_TTF REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_LIB_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIR})
include_directories(${SDL2_TTF_LIB_DIR})
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
include_directories(${SDL2_IMAGE_LIB_DIR})
include_directories(tinyfiledialogs/tinyfiledialogs.h)

set(CMAKE_C_STANDARD 99)

add_executable(SDLProject main.c SDLfunctions.h functions.h tinyfiledialogs/tinyfiledialogs.c)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lcomdlg32 -lole32 -LC:/mingw/lib -lurlmon -mwindows -lmingw32) #-mwindows -lmingw32 // -lm
message(STATUS "${SDL2_LIBRARY} = ${${SDL2_LIBRARY}}")