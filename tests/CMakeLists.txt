cmake_minimum_required(VERSION 3.18)


project(SCADA_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


    include(FetchContent)

    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1 # Or a specific branch like 'master'
    )
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(SFML)

    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )

    FetchContent_MakeAvailable(googletest)

set(YOUR_SOURCES
    ../src/gui_widgets.cpp
    ../src/scada_app.cpp
    ../tests/test.cpp
)

set(YOUR_HEADERS
    ../src/scada_app.hpp 
)

add_executable(SCADA_test ${YOUR_SOURCES} ${YOUR_HEADERS})

target_link_libraries(SCADA_test PRIVATE 
        sfml-graphics 
        sfml-window 
        sfml-system
        gtest_main
)