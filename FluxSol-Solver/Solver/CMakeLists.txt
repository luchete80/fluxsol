set (NAME Solver)

set (HEADERS 
Solver.h
SIMPLE.h
PETSC_Solver.h
PETSC_GAMGSolver.h
)

set (SOURCES 
#Simple.cpp
PETSC_Solver.cpp
PETSC_GAMGSolver.cpp
)

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )
add_library(${NAME} OBJECT ${SOURCES} )

