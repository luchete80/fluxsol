set (NAME windows_)

file(GLOB all_SRC
    "*.cpp"
)

#set_source_files_properties(${all_SRC}  PROPERTIES LANGUAGE CXX )

add_library(${NAME} OBJECT ${all_SRC} )