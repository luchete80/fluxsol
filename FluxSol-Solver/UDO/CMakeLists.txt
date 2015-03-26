set (NAME UDO)
set (SOURCES 
FileSearch.cpp
)

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )
add_library(${NAME} OBJECT ${SOURCES} )