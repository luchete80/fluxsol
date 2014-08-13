set (NAME Output)

set (SOURCES 
OutputFile.cpp
WriteVtk_FvGrid.cpp
)

add_library(${NAME} STATIC ${SOURCES} )

install (FILES ${NAME}.h DESTINATION include)
install (FILES lib${NAME}.a DESTINATION lib)
 
