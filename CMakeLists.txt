cmake_minimum_required(VERSION 3.20)
project(Visualise)

set(CMAKE_CXX_STANDARD 17)

add_executable(Visualise main.cpp Led.cpp Led.h)

include_directories(C:/Users/Erik/Documents/libs/SFML-2.5.1-windows-vc15-32-bit/SFML-2.5.1/include)

set(SFML_ROOT C:/Users/Erik/Documents/libs/SFML-2.5.1-windows-vc15-32-bit/SFML-2.5.1)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Visualise ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()