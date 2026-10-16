cmake_minimum_required(VERSION 3.20)
project(GameSimple C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Beberapa library pihak ketiga masih memakai cmake_minimum_required versi lama
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# ---------------------------------------------------------------------------
# Unduh raylib dan raygui otomatis
# ---------------------------------------------------------------------------
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

# raygui hanya berupa satu header, jadi tidak perlu add_subdirectory
FetchContent_Declare(raygui
    URL https://github.com/raysan5/raygui/archive/refs/tags/4.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    SOURCE_SUBDIR tidak-ada)

FetchContent_MakeAvailable(raylib raygui)

# ---------------------------------------------------------------------------
# Program game
# ---------------------------------------------------------------------------
file(GLOB CORE_SOURCES ${CMAKE_SOURCE_DIR}/core/*.cpp)

add_executable(game main.cpp ui/App.cpp ${CORE_SOURCES})
target_include_directories(game PRIVATE ${CMAKE_SOURCE_DIR})
# SYSTEM: sembunyikan warning dari dalam raygui.h (bukan kode kita)
target_include_directories(game SYSTEM PRIVATE ${raygui_SOURCE_DIR}/src)
target_link_libraries(game PRIVATE raylib)

# Link statis supaya game.exe bisa jalan tanpa DLL MinGW
if(MINGW)
    target_link_options(game PRIVATE -static -static-libgcc -static-libstdc++)
endif()

# Salin folder assets/ ke samping game.exe setiap selesai build
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:game>/assets)
