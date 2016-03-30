set (NAME Mesh)

set (SOURCES 
Cell.cpp
Face.cpp
FluentMesh.cpp
Grid.cpp
MeshInfo.cpp
Node.cpp
ReadCGNS.cpp
Rectangular.cpp
Structured.cpp
Vertex.cpp
)
#Needed OBJECT to make independed linkeable library
add_library(${NAME} OBJECT ${SOURCES} )
#add_library(${NAME} STATIC ${SOURCES} )

#install (FILES ${NAME}.h DESTINATION include)
#install (FILES lib${NAME}.a DESTINATION lib)
 
