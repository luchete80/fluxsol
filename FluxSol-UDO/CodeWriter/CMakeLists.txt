cmake_minimum_required(VERSION 2.6)
include (CheckSymbolExists)
include (CheckCXXSymbolExists)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

include ("$ENV{FLUXSOL_DIR}/Src/defs.cmake")

set (NAME CodeWriter)
project(${NAME})

set (FLUXSOL_DIR "$ENV{FLUXSOL_DIR}")
set (FLUXSOL_LIB_DIR ${FLUXSOL_DIR}/Bin/FluxSol)
set (FLUXSOL_SRC_DIR "$ENV{FLUXSOL_DIR}/Src/FluxSol-Solver")



set (SOURCES 
FileSearch.cpp
${FLUXSOL_SRC_DIR}/Input/Input.cpp
${FLUXSOL_SRC_DIR}/Field/Field.cpp
${FLUXSOL_SRC_DIR}/Field/PatchField.cpp
${FLUXSOL_SRC_DIR}/Utils/Utils.cpp
${FLUXSOL_SRC_DIR}/SistEcuac/SistEcuac.cpp
${FLUXSOL_SRC_DIR}/Field/BoundaryField.cpp
${FLUXSOL_SRC_DIR}/Type/Scalar.cpp
${FLUXSOL_SRC_DIR}/Type/Vec3D.cpp
dummylib.cpp
main.cpp
)



FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )
install (FILES ${NAME}.h DESTINATION include)

add_executable(${NAME} ${SOURCES})
#target_link_libraries (${NAME} ${FLUXSOL_LIB} ${EXT_LIBS})
target_link_libraries (${NAME} fluxsol-FvMesh ${EXT_LIBS})