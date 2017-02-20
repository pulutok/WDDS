cmake_minimum_required(VERSION 3.6)
project(WDDS)
set(CMAKE_CXX_STANDARD 11)

include(cmake-build-debug/FindTins.cmake)
include(cmake-build-debug/FindPCAP.cmake)
include(cmake-build-debug/FindMySQL.cmake)

set(SOURCE_FILES main.cpp Scanner.cpp Scanner.h CustomDefines.h WDDS.cpp WDDS.h DBConnector.cpp DBConnector.h)
add_executable(WDDS ${SOURCE_FILES})

if(PCAP_FOUND)
    message(STATUS "PCAP_INCLUDE_DIRS: ${PCAP_INCLUDE_DIR}")
    message(STATUS "PCAP_LIBRAIES: ${PCAP_LIBRARY}")
    include_directories(${PCAP_INCLUDE_DIR})
    target_link_libraries(WDDS ${PCAP_LIBRARY})
endif()

find_package(Boost 1.58.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(WDDS ${Boost_LIBRARIES})
endif()

if(MYSQL_FOUND)
    message(STATUS "MYSQL_INCLUDE_DIRS: ${MYSQL_INCLUDE_DIR}")
    message(STATUS "MYSQL_LIBRARIES: ${MYSQL_LIBRARIES}")

    include_directories(${MYSQL_INCLUDE_DIR})
    target_link_libraries(WDDS ${MYSQL_LIBRARIES})
endif()

if(TINS_FOUND)
    message(STATUS "TINS_INCLUDE_DIR: ${TINS_INCLUDE_DIR}")
    message(STATUS "TINS_LIBRARIES: ${TINS_LIBRARIES}")
    include_directories(${TINS_INCLUDE_DIR})
    target_link_libraries(WDDS ${TINS_LIBRARIES})
endif()