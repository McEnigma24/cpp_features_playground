cmake_minimum_required(VERSION 3.18)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(LIBRARY_PATH_CORE ${CMAKE_CURRENT_SOURCE_DIR}/external/CORE_lib/build/)
link_directories(${LIBRARY_PATH_CORE})

include_directories(_inc external/CORE_lib/_inc)
aux_source_directory(_src SOURCES)
aux_source_directory(_src/base SOURCES)

aux_source_directory(_src TEST_SOURCES)
aux_source_directory(_src/base TEST_SOURCES)
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*main\\.cpp")
aux_source_directory(_test TEST_SOURCES)

# COMPILER FLAGS #

# https://docs.oracle.com/cd/E19957-01/806-3572/Rev.html
# https://caiorss.github.io/C-Cpp-Notes/compiler-flags-options.html
# https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wfatal-errors

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -g -Wall -Wextra")                                                             # to see everything
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -g -Wfatal-errors -Werror=uninitialized -Werror=init-self -Werror=reorder")    # for normal coding
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -march=native -w")                                                         # for release

set(CONST_TARGET_NAME "TEMPLATE")

option(BUILD_LIBRARY "Build library instead of library executable" OFF) # default value - could be overwritten by provided FLAG

if(BUILD_LIBRARY)
    message(WARNING "Building library")
    
    set(TARGET_NAME "${CONST_TARGET_NAME}")
    add_library(${TARGET_NAME} SHARED ${SOURCES})
    target_compile_definitions(${TARGET_NAME} PRIVATE LINUX)
    set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
    target_link_libraries(${TARGET_NAME} PUBLIC 
        ${LIBRARY_PATH_CORE}/libCORE.a
    )
else()
    message(WARNING "Building exe")

    set(TARGET_NAME "${CONST_TARGET_NAME}.exe")
    add_executable(${TARGET_NAME} ${SOURCES})
    target_compile_definitions(${TARGET_NAME} PRIVATE LINUX BUILD_EXECUTABLE)
    set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "-m64" LINK_FLAGS "-m64")
    target_link_libraries(${TARGET_NAME} PUBLIC 
        ${LIBRARY_PATH_CORE}/libCORE.a
    )
endif()

option(CTEST_ACTIVE "Testing is active" OFF)  # default value - could be overwritten by provided FLAG

if(CTEST_ACTIVE)
    message(WARNING "TESTING...")
    # Włączenie testów
    enable_testing()

    # Pobieranie jako submoduł
    add_subdirectory(external/googletest)

    # Dodanie testów
    set(TEST_EXE_NAME "test.texe")  # Nazwa binarki testowej
    add_executable(${TEST_EXE_NAME} ${TEST_SOURCES})
    target_link_libraries(${TEST_EXE_NAME} PRIVATE
        gtest_main
        ${LIBRARY_PATH_CORE}/libCORE.a
    )
    # Rejestracja testów
    include(GoogleTest)
    gtest_discover_tests(${TEST_EXE_NAME})

endif()