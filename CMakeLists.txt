cmake_minimum_required(VERSION 3.0.0)
project(rendering VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("${CMAKE_SOURCE_DIR}\\include")


set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

set(HEADER_FILES 
include/headers/data.h
include/headers/func.h
include/headers/shader.h
include/glad/glad.h
)

set(SOURCE 
src/main.cpp
src/funcs/func.cpp
src/funcs/data.cpp
src/funcs/shader.cpp
)

#EXPERIMENT!!!!!

set(LIBRARY 
OpenGL::GL
glad
glfw
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


add_library(glad STATIC src/glad.c)
add_library(shader src/funcs/shader.cpp ${HEADER_FILES}) #DO THIS TO INCLUDE ALL HEADERS FOR THIS PARTICULAR LIBRARY/CPP FILE TO USE.

add_executable(rendering "${SOURCE}")
#target_include_directories(rendering PUBLIC "${CMAKE_SOURCE_DIR}\\include") #UNSURE WHETHER NEEDED
target_link_libraries(rendering "${LIBRARY}")


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
