cmake_minimum_required(VERSION 3.6)
project(WDDS)
set(CMAKE_CXX_STANDARD 11)

include(cmake-build-debug/FindPCAP.cmake)

set(SOURCE_FILES main.cpp Scanner.cpp Scanner.h customDefines.cpp WDDS.cpp WDDS.h DBConnector.cpp DBConnector.h)
add_executable(WDDS ${SOURCE_FILES})

if(PCAP_FOUND)
    message(STATUS "PCAP_INCLUDE_DIRS: ${PCAP_INCLUDE_DIR}")
    message(STATUS "PCAP_LIBRAIES: ${PCAP_LIBRARY}")
    include_directories(${PCAP_INCLUDE_DIR})
    target_link_libraries(WDDS ${PCAP_LIBRARY})
endif()


find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(WDDS ${Boost_LIBRARIES})
endif()



