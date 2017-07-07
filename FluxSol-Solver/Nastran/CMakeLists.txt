set (NAME Nastran)

set (SOURCES 
Cadenas.cpp
Caract.cpp
Elementos.cpp
Material.cpp
Modelo.cpp
Nastran.cpp
Nodo.cpp
Prop.cpp
SistCoord.cpp
)

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )

add_library(${NAME} OBJECT ${SOURCES} ${_header})