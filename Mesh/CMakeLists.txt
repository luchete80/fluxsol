set (NAME Mesh)

set (SOURCES 
Cell.cpp
Face.cpp
Grid.cpp
MeshInfo.cpp
Node.cpp
ReadCGNS.cpp
Rectangular.cpp
Structured.cpp
Vertex.cpp
)

add_library(${NAME} STATIC ${SOURCES} )

install (FILES ${NAME}.h DESTINATION include)
install (FILES lib${NAME}.a DESTINATION lib)
 
