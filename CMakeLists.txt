project (cis600cpp)

SET(SOURCES cis600hw3.cpp
            ColumnCount.cpp
            CellularAutomaton.cpp
            AutomatonCollection.cpp
)

add_definitions("-Wall -pedantic -O2 -g -std=c++14")

FIND_PACKAGE(Threads)

add_executable(hw3 ${SOURCES})

target_link_libraries (hw3 ${CMAKE_THREAD_LIBS_INIT})
