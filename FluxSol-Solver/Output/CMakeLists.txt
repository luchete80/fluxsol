set (NAME Output)

set (SOURCES 
OutputFile.cpp
WriteVtk_FvGrid.cpp
)

add_library(${NAME} OBJECT ${SOURCES} )
