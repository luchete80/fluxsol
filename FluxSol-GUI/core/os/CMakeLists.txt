set (NAME coreos_)

file(GLOB all_SRC
    "*.cpp"
)

add_library(${NAME} OBJECT ${all_SRC} )