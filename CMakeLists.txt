cmake_minimum_required(VERSION 3.0.0)
project(HahaNetLib VERSION 0.1.0)
include(CTest)
enable_testing()
add_subdirectory(Base)
add_subdirectory(Buffer)
add_subdirectory(Net)

add_executable(HahaNetLib main.cpp)
target_link_libraries(HahaNetLib PUBLIC HahaLibBase HahaLibNet HahaLibBuffer)
target_include_directories(HahaNetLib PUBLIC 
"${PROJECT_BINARY_DIR}" 
"/home/paruru/download/boost_1_80_0"
"/home/paruru/download/url/include"
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")