cmake_minimum_required (VERSION 3.20)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("sizzle")

find_package(SDL3 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
  "src/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
  "include/*.h"
)

add_executable(sizzle ${SOURCES} ${HEADERS})

target_link_libraries(sizzle PRIVATE SDL3::SDL3)

target_include_directories(sizzle PRIVATE include/sizzle)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sizzle PROPERTY CXX_STANDARD 20)
endif()
