cmake_minimum_required(VERSION 3.25)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

#fetch noshell
FetchContent_Declare(noshell
        GIT_REPOSITORY https://github.com/gmarcais/NoShell.git
        GIT_TAG master
)

FetchContent_MakeAvailable(noshell)

project(tlp-tui)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp
        scripts.cpp
        scripts.h)

# link static library
target_link_libraries(${PROJECT_NAME}
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE noshell-static
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
