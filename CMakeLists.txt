cmake_minimum_required(VERSION 3.10)
project(carpet_server)

set(CMAKE_CXX_STANDARD 17)

set(Boost_ARCHITECTURE -x64)
set(BOOST_ROOT "D:/Dev/boost-1_68")
set(BOOST_INCLUDEDIR "D:/Dev/boost-1_68/include")
set(BOOST_LIBRARYDIR "D:/Dev/boost-1_68/lib")
set(BOOST_NO_SYSTEM_PATHS ON)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS log program_options system)

add_definitions( -DCARPET_SERVER )

add_executable(carpet_server start.cpp constants/server_constants.h handler/session.cpp handler/session.h handler/manager.cpp handler/manager.h tools/cplogger.cpp tools/cplogger.h constants/packet_opcodes.h packet/packethelper.cpp packet/packethelper.h entity/car.cpp entity/car.h entity/entity.cpp entity/entity.h packet/packet.cpp packet/packet.h)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    message("Boost VERSION: ${Boost_VERSION}")
    message("Boost INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message("Boost Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    message("Boost LIBRARIES: ${Boost_LIBRARIES}")

    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES})
    if(MINGW)
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} ws2_32)
    endif()
endif()