cmake_minimum_required(VERSION 3.0)
if(UNITTEST AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
    file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h
            ${CMAKE_CURRENT_BINARY_DIR}/*.pb.*)

    add_executable(${PROJECT_NAME}_t ${SRC})
	
    target_include_directories(${PROJECT_NAME}_t PRIVATE
        ${${QBOX_PROJECT_NAME}_SOURCE_DIR}/BaseTest/qtest
        ${${QBOX_PROJECT_NAME}_SOURCE_DIR}/BaseTest/gtest/gtest/include
        ${${QBOX_PROJECT_NAME}_SOURCE_DIR}/BaseTest/gtest/gmock/include
        ${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_DIR}
        )

    target_link_libraries(${PROJECT_NAME}_t ${PROJECT_NAME} gtest ipps ippcore ippvm)
    set_target_properties(${PROJECT_NAME}_t PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/test
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/test
        )
	add_test(NAME ${PROJECT_NAME}_t COMMAND ${PROJECT_NAME}_t)
		if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/config/conf.cmake)
		message("configure test")
		include(${CMAKE_CURRENT_SOURCE_DIR}/test/config/conf.cmake)
	endif()
endif()
