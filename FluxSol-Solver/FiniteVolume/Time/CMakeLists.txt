set (NAME FvTime)
set (SOURCES 
FvImpTime.cpp
)

add_library(${NAME} OBJECT ${SOURCES} )

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )

