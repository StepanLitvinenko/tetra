set(CMAKE_CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD 14)

set(LIB_LEVEL1 NOTFOUND)
set(LIB_LEVEL2 NOTFOUND)
set(LIB_LEVEL3 NOTFOUND)
set(PROC_LIBS_ROOT NOTFOUND)
set(CLEAN_ALL ${CMAKE_CURRENT_LIST_DIR}/cmake/clean-all.cmake)
set(RM "/bin/rm")
function(definePaths ROOT_PATH)
    set(LIB_LEVEL1 ${ROOT_PATH}/Level1NativeLibSo PARENT_SCOPE)
    set(LIB_LEVEL2 ${ROOT_PATH}/Level2NativeLibSo  PARENT_SCOPE)
    set(LIB_LEVEL3 ${ROOT_PATH}/Level3NativeLibSo PARENT_SCOPE)
    set(PROC_LIBS_ROOT  ${ROOT_PATH}/ PARENT_SCOPE)
endfunction(definePaths)

function(cleanAll)
	if(NOT TARGET clean-all)
		if(NOT ${CMAKE_HOME_DIRECTORY} STREQUAL ${CMAKE_BINARY_DIR})	
            add_custom_target(clean-all COMMAND ${CMAKE_BUILD_TOOL} clean COMMAND ${RM} -rf ${CMAKE_BINARY_DIR}/*)
        else()
			add_custom_target(clean-all COMMAND ${CMAKE_BUILD_TOOL} clean COMMAND ${CMAKE_COMMAND} -P ${CLEAN_ALL})
		endif()
    endif()
endfunction(cleanAll)
definePaths(${CMAKE_CURRENT_LIST_DIR})