set (NAME coreio_)

file(GLOB all_SRC
    "*.cpp"
)

file(GLOB all_CSRC
    "*.c"
)

add_library(${NAME} OBJECT ${all_SRC} ${all_CSRC})