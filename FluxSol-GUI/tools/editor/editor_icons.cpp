#include "editor_icons.h"
#include "scene/resources/theme.h"

// pixmaps = glob.glob("*.png")


// for x in pixmaps:

	// var_str=x[:-4]+"_png";

	// f.write("static const unsigned char "+ var_str +"[]={\n");

	// pngf=open(x,"rb");

	// b=pngf.read(1);
	// while(len(b)==1):
		// f.write(hex(ord(b)))
		// b=pngf.read(1);
		// if (len(b)==1):
			// f.write(",")

	// f.write("\n};\n\n\n");
	// pngf.close();

static Ref<ImageTexture> make_icon(const uint8_t* p_png) {
	Ref<ImageTexture> texture( memnew( ImageTexture ) );
	texture->create_from_image( Image(p_png),ImageTexture::FLAG_FILTER );
	return texture;
}


void editor_register_icons(Ref<Theme> p_theme) {

	//for x in pixmaps:

	// type=x[5:-4].title().replace("_","");
	// var_str=x[:-4]+"_png";
	// f.write("\tp_theme->set_icon(\""+type+"\",\"EditorIcons\",make_icon("+var_str+"));\n");

}