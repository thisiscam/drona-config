cmake_minimum_required (VERSION 3.1)

file(GLOB SRC *.cpp *.h)

add_executable(test_parser ${SRC})

add_definitions( -DTEST_WS_PARSER )
