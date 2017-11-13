cmake_minimum_required(VERSION 3.1)

project(lruc)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Download and unpack googletest at configure time
configure_file(3rdparty/googletest/CMakeLists.txt googletest-download/CMakeLists.txt)
execute_process(
	COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}"
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(
	COMMAND "${CMAKE_COMMAND}" --build .
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(
	"${CMAKE_BINARY_DIR}/googletest-src"
	"${CMAKE_BINARY_DIR}/googletest-build"
)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
	include_directories(
		"${gtest_SOURCE_DIR}/include"
		"${gmock_SOURCE_DIR}/include"
	)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES src/lru_cache_test.cpp)

set(PROJECT_TEST_TARGET_NAME ${PROJECT_NAME}_test)
add_executable(${PROJECT_TEST_TARGET_NAME} ${SOURCES})
target_link_libraries(${PROJECT_TEST_TARGET_NAME} gtest)

enable_testing()
add_test(NAME ${PROJECT_NAME}Test COMMAND ${PROJECT_TEST_TARGET_NAME})
