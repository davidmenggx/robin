cmake_minimum_required(VERSION 3.21)

if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug>:EditAndContinue>$<$<NOT:$<CONFIG:Debug>>:ProgramDatabase>")
endif()

project("robin" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.4.1 
)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        preview-3.1.6 
)

FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        preview-3.1.0
)

FetchContent_MakeAvailable(cli11 SDL3 SDL3_ttf)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h")

add_executable(robin ${SOURCES} ${HEADERS})

target_link_options(robin PRIVATE $<$<CONFIG:RelWithDebInfo>:/PROFILE>)

target_link_libraries(robin PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf CLI11::CLI11)
target_include_directories(robin PRIVATE include)

set_property(TARGET robin PROPERTY CXX_STANDARD 20)
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(TARGET robin POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:robin>
    $<TARGET_FILE_DIR:robin>
  COMMAND_EXPAND_LISTS
)
