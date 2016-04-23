set (NAME Model)
project(${NAME})
set (SOURCES 
Model.cpp
StatThermalModel.cpp
StatThermoFluidModel.cpp
)

set (FLUXSOL_LIB_DIR $ENV{FLUXSOL_LIB_DIR}) 

#Needed OBJECT to make independed linkeable library
add_library(${NAME} OBJECT ${SOURCES})
#add_library(${NAME} STATIC ${SOURCES} )

install (FILES ${NAME}.h DESTINATION include/FluxSol-Solver/Model)
#install (FILES lib${NAME}.a DESTINATION lib)