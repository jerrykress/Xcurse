cmake_minimum_required(VERSION 3.0.0)
project(Xcurse VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

file(GLOB src_files CONFIGURE_DEPENDS
     "src/*.cpp"
     "src/Common/*.cpp"
     "src/Display/*.cpp"
     "src/Utils/*.cpp"
     "src/Widgets/*.cpp"
     "src/Window/*.cpp"
)

add_executable(Xcurse main.cpp ${src_files})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
