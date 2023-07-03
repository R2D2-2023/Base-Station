cmake_minimum_required(VERSION 3.20)
project(LEDMatrix_R2D2)

set(CMAKE_CXX_STANDARD 17)
add_executable(LEDMatrix_R2D2 main.cpp Led.cpp Led.h Matrix.cpp Matrix.h ColorWheel.cpp ColorWheel.h Painting.cpp Painting.h Dropbox.cpp Dropbox.h json.hpp)

include_directories(c:/TIBestanden/SFML-2.5.1/include)

set(CURL_INCLUDE_DIR "C:/curl/include/curl")
include_directories(${CURL_INCLUDE_DIR})
set(CURL_LIBRARY "C:/curl/lib/libcurl.dll.a")  # Specify the path to libcurl.dll
set(CURL_LIBRARY "C:/curl/bin/libcurl.dll")  # Specify the path to libcurl.dll
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(LEDMatrix_R2D2 ${CURL_LIBRARIES})

set(SFML_ROOT c:/TIBestanden/SFML-2.5.1)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(LEDMatrix_R2D2 ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build
if(WIN32)
    file(GLOB CURL_DLLS "C:/curl/lib/*.dll")
    file(COPY ${CURL_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB CURL_DLLS "C:/curl/bin/*.dll")
    file(COPY ${CURL_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()