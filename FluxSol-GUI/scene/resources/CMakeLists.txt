set (NAME sceneresources_)

file(GLOB all_SRC
    "*.cpp" "*.c"
)

add_library(${NAME} OBJECT ${all_SRC} )