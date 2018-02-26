cmake_minimum_required(VERSION 3.0)

project(CraftGE C CXX)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

# if (WIN32)
#   set(CMAKE_SHARED_LIBRARY_PREFIX "")
# endif ()

# find_package(glfw3)

include_directories(include)
link_directories(lib)


if (WIN32)
  file(GLOB PROJECT_LINK_LIBS "lib/*.lib")
  file(GLOB PROJECT_LINK_DLLS "lib/*.dll")
  file(COPY ${PROJECT_LINK_DLLS}
          DESTINATION ${PROJECT_BINARY_DIR}/Debug)
else()
  file(GLOB PROJECT_LINK_LIBS "lib/*.so")
endif()

if (WIN32)
  file(COPY res DESTINATION ${PROJECT_BINARY_DIR}/Debug)
  file(COPY shader DESTINATION ${PROJECT_BINARY_DIR}/Debug)
  file(COPY obj DESTINATION ${PROJECT_BINARY_DIR}/Debug)
endif()

file(COPY res DESTINATION ${PROJECT_BINARY_DIR})
file(COPY shader DESTINATION ${PROJECT_BINARY_DIR})
file(COPY obj DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB SOURCES "src/*.cpp"
                  "src/*.c")

file(GLOB INCLUDES "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

source_group("Include Files" FILES ${INCLUDES})
#source_group("Source" FILES ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS})