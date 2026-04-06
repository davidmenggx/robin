cmake_minimum_required (VERSION 3.20)

if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("robin")

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
	"src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
	"include/*.h"
)

add_executable(robin ${SOURCES} ${HEADERS})

target_link_libraries(robin PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf CLI11::CLI11)

target_include_directories(robin PRIVATE include/robin)

set_property(TARGET robin PROPERTY CXX_STANDARD 20)

add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
