set (NAME editorioplugins_)

file(GLOB all_SRC
    "*.cpp"
)

add_library(${NAME} OBJECT ${all_SRC} #"../../../scene/2d/node_2d.cpp"
)

