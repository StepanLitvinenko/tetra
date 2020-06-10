cmake_minimum_required(VERSION 2.8)

project(tetraProj)
add_executable(${PROJECT_NAME} "main.cpp")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wno-strict-aliasing")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBTMS_ROOT ${LIBTMS_ROOT} ${CMAKE_CURRENT_SOURCE_DIR}/LibTms)

set(BIN_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Install/Bin)
set(EXTRA_LIBS)

add_definitions(-DTMS320C66X -DLITTLE_ENDIAN_HOST -DLITTLE_ENDIAN -DNOASSUME)
add_definitions(-DPC_SIMULATOR)

add_custom_target(${PROJECT_NAME}_link_target ALL)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tetra_source/Level2NativeLibSo/TetraLogicalProc TetraLogicalProc )
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tetra_source/Level3NativeLibSo/CommonHostLib)
#set(SOURCES
  #      tetra_source/Level2NativeLibSo/TetraLogicalProc./tetra_dl_stream.cpp
   #     )

#set(HEADERS
      #  tetra_source/Level2NativeLibSo/TetraLogicalProc./tetra_dl_stream.h
     #   )


target_include_directories(${PROJECT_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_INSTALL_DIR} ${CMAKE_BINARY_DIR}
        )

#add_library(system STATIC ${SOURCES} ${HEADERS})

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_link_target)

install(TARGETS ${PROJECT_NAME} DESTINATION ${BIN_INSTALL_DIR})
