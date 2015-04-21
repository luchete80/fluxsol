set (NAME Solver)

set (SOURCES 
Simple.cpp
PETSC_Solver.cpp
)

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )
add_library(${NAME} OBJECT ${SOURCES} )

