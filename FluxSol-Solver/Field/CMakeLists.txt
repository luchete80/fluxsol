set (NAME Field)

set (HEADERS 
Field.h
FieldOperations.h
FvField.h
GeometricField.h
)

set (SOURCES 
BoundaryField.cpp
PatchField.cpp
Field.cpp
_CC_Fv_field.cpp
)	

FILE(GLOB _header
  ${CMAKE_SOURCE_DIR}/${NAME}/*.h
  )
  
add_library(${NAME} OBJECT ${SOURCES} ${HEADERS})
 
