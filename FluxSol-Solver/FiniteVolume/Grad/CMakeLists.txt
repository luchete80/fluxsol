set (NAME FvGrad)

set (SOURCES 
FvexpGrad.cpp
)

#Needed OBJECT to make independed linkeable library
add_library(${NAME} OBJECT ${SOURCES} )
#add_library(${NAME} STATIC ${SOURCES} )

#install (FILES ${NAME}.h DESTINATION include)
#install (FILES lib${NAME}.a DESTINATION lib)
 
