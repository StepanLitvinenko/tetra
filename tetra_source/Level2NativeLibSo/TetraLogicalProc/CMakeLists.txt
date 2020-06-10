cmake_minimum_required(VERSION 3.0)
project (TetraLogicalProc)

include("../../proclibs.cmake")
cleanAll()
if(NOT TARGET ${PROJECT_NAME})
       # for fefacroring code purpose
       # add_definitions(-DHOST_CODE)
       #for reoulils lib
        add_definitions(-DSIMULATION)
        add_definitions(-DTMS320C64X)
        add_definitions(-DLITTLE_ENDIAN_HOST)
       #====================
        include_directories(${PROC_LIBS_ROOT})
	include_directories(${LIB_LEVEL3}/ViterbiDecoder)
        include_directories(${LIB_LEVEL3}/CommonHostLib)
        include_directories(${LIB_LEVEL3}/reoutils)
        include_directories(${LIB_LEVEL2})
        include_directories(${LIB_LEVEL3})
	file(GLOB  SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
        #add_subdirectory(${LIB_LEVEL3}/ViterbiDecoder ${CMAKE_CURRENT_BINARY_DIR}/ViterbiDecoder)4
        set(SOURCES
                tetra_source/Level2NativeLibSo/TetraLogicalProc./tetra_dl_stream.cpp
                )

        set(HEADERS
                tetra_source/Level2NativeLibSo/TetraLogicalProc./tetra_dl_stream.h
                )



        #add_library(system STATIC ${SOURCES} ${HEADERS})
        add_subdirectory(${LIB_LEVEL3}/reoutils ${CMAKE_CURRENT_BINARY_DIR}/reoutils)
        add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
        target_link_libraries(${PROJECT_NAME}  viterbidecoder reoutils)
endif()
