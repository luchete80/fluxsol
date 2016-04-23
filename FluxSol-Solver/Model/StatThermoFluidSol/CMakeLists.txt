set (NAME StatThermoFluidModel)
project(${NAME})
set (SOURCES 
Model.cpp
StatThermoFluidSol.cpp
)

set (EPSOL_LIB_DIR $ENV{EPSOL_LIB_DIR}) 
add_executable(${NAME} STFSol_main.cpp)
target_link_libraries (${NAME} fluxsol-Model fluxsol-Type)
install(TARGETS ${NAME}
     RUNTIME DESTINATION bin)

install (FILES ${NAME}.h DESTINATION include/FluxSol-Solver/Model)

 
