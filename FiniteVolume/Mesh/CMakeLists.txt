set (NAME FvMesh)
set (SOURCES 
CC_Cell.cpp
FvFace.cpp
FvGrid.cpp
MeshInfo.cpp
)

add_library(${NAME} STATIC ${SOURCES} )

install (FILES ${NAME}.h DESTINATION include)
install (FILES lib${NAME}.a DESTINATION lib)
 
