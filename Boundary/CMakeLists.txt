set (NAME Boundary)
set (SOURCES 
Boundary.cpp
)

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )

add_library(${NAME} OBJECT ${SOURCES} ${_header})
