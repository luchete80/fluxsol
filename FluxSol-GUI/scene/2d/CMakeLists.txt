set (NAME scene2d_)

# file(GLOB all_SRC
    # "canvas_item.cpp"
# )

 set(all_SRC
    canvas_item.cpp
	node_2d.cpp
	#visibility_notifier_2d.cpp
)

add_library(${NAME} OBJECT ${all_SRC} )