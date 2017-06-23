set (NAME scene3d_)

# file(GLOB all_SRC
    # "canvas_item.cpp"
# )

 set(all_SRC
	camera.cpp
	mesh_instance.cpp
	spatial.cpp
	visual_instance.cpp
)

add_library(${NAME} OBJECT ${all_SRC} )