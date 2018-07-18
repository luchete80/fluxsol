set (NAME Tests)

include (../../defs.cmake)

include_directories(${CGNS_INCLUDE_DIRS})
include_directories(${ALGLIB_INCLUDE_DIRS})

set (SOURCES 
CGNSTest.cpp
)

add_library(${NAME} OBJECT ${SOURCES} )
 
