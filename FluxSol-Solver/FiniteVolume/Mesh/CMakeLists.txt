set (NAME FvMesh)
set (SOURCES 
CC_Cell.cpp
FvFace.cpp
FvGrid.cpp
GridPartition.cpp
MeshInfo.cpp
)

add_library(${NAME} OBJECT ${SOURCES} )

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )

#install (FILES ${NAME}.h DESTINATION include)
#install (FILES lib${NAME}.a DESTINATION lib)
 
