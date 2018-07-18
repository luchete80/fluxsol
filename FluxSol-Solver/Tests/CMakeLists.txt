set (NAME Tests)

include (../../defs.cmake)

include_directories(${CGNS_INCLUDE_DIRS})
include_directories(${ALGLIB_INCLUDE_DIRS})

set (SOURCES 
CGNSTest.cpp
)

link_directories("${FLUXSOL_DIR}/Bin_Tests")


add_executable(${NAME} ${SOURCES} )

target_link_libraries (${NAME}    UDO fluxsol-IO fluxsol-FvMesh fluxsol-Model fluxsol-Solver fluxsol-Laplacian fluxsol-FvGrad fluxsol-Field fluxsol-EqnSys fluxsol-Type)
 
