/*************************************************************************/
/*  register_scene_types.cpp                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "register_scene_types.h"
#include "os/os.h"
#include "globals.h"
#include "scene/io/resource_format_image.h"
#include "scene/io/resource_format_wav.h"

#include "resources/default_theme/default_theme.h"
#include "object_type_db.h"
#include "scene/main/canvas_layer.h"
#include "scene/main/instance_placeholder.h"
#include "scene/main/viewport.h"
#include "scene/main/http_request.h"
#include "scene/gui/control.h"
#include "scene/gui/texture_progress.h"
#include "scene/gui/button.h"
#include "scene/gui/link_button.h"
#include "scene/gui/button_array.h"
#include "scene/gui/button_group.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/scroll_bar.h"
#include "scene/gui/progress_bar.h"
#include "scene/gui/slider.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/option_button.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/texture_frame.h"
#include "scene/gui/patch_9_frame.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/check_box.h"
#include "scene/gui/check_button.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/tabs.h"
#include "scene/gui/center_container.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/panel.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/tree.h"
#include "scene/gui/item_list.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_button.h"
#include "scene/gui/separator.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/box_container.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/split_container.h"

#include "scene/2d/canvas_item.h"


#include "scene/main/scene_main_loop.h"
#include "scene/main/resource_preloader.h"
#include "scene/resources/packed_scene.h"
#include "scene/main/scene_main_loop.h"


#include "scene/resources/scene_preloader.h"
#include "scene/resources/dynamic_font.h"
#include "scene/resources/dynamic_font_stb.h"

#include "scene/main/timer.h"

#include "scene/resources/polygon_path_finder.h"

#include "scene/resources/texture.h"

#include "scene/resources/room.h"

#include "scene/resources/shader_graph.h"

#include "scene/resources/bit_mask.h"
#include "scene/resources/color_ramp.h"
#include "scene/scene_string_names.h"


static ResourceFormatLoaderImage *resource_loader_image=NULL;
//static ResourceFormatLoaderWAV *resource_loader_wav=NULL;


#ifdef TOOLS_ENABLED


#endif
static ResourceFormatLoaderTheme *resource_loader_theme=NULL;
static ResourceFormatLoaderShader *resource_loader_shader=NULL;

//static ResourceFormatSaverText *resource_saver_text=NULL;
//static ResourceFormatLoaderText *resource_loader_text=NULL;

static ResourceFormatLoaderDynamicFont *resource_loader_dynamic_font=NULL;

//static SceneStringNames *string_names;

void register_scene_types() {

	SceneStringNames::create();

	OS::get_singleton()->yield(); //may take time to init

	Node::init_node_hrcr();

	resource_loader_image = memnew( ResourceFormatLoaderImage );
	ResourceLoader::add_resource_format_loader( resource_loader_image );

	//resource_loader_wav = memnew( ResourceFormatLoaderWAV );
//	ResourceLoader::add_resource_format_loader( resource_loader_wav );
//	resource_loader_dynamic_font = memnew( ResourceFormatLoaderDynamicFont );
//	ResourceLoader::add_resource_format_loader( resource_loader_dynamic_font );

#ifdef TOOLS_ENABLED

	//scene first!

#endif

	resource_loader_theme = memnew( ResourceFormatLoaderTheme );
	ResourceLoader::add_resource_format_loader( resource_loader_theme );

	resource_loader_shader = memnew( ResourceFormatLoaderShader );
	ResourceLoader::add_resource_format_loader( resource_loader_shader );

    print_line("Making default theme"); //LUCIANO

	make_default_theme();
   print_line("Make default theme"); //LUCIANO
	OS::get_singleton()->yield(); //may take time to init

	ObjectTypeDB::register_type<Object>();

	ObjectTypeDB::register_type<Node>();
	ObjectTypeDB::register_virtual_type<InstancePlaceholder>();

//	ObjectTypeDB::register_type<Viewport>();
//	ObjectTypeDB::register_virtual_type<RenderTargetTexture>();
//	ObjectTypeDB::register_type<HTTPRequest>();
//	ObjectTypeDB::register_type<Timer>();
//	ObjectTypeDB::register_type<CanvasLayer>();
//	ObjectTypeDB::register_type<CanvasModulate>();
//	ObjectTypeDB::register_type<ResourcePreloader>();
//
   print_line("Registering GUI"); //LUCIANO
	/* REGISTER GUI */
	ObjectTypeDB::register_type<ButtonGroup>();
	ObjectTypeDB::register_virtual_type<BaseButton>();

	OS::get_singleton()->yield(); //may take time to init

	ObjectTypeDB::register_type<Control>();
//	ObjectTypeDB::register_type<EmptyControl>();
	ObjectTypeDB::add_compatibility_type("EmptyControl","Control");
	ObjectTypeDB::register_type<Button>();
	ObjectTypeDB::register_type<Label>();
	ObjectTypeDB::register_type<HScrollBar>();
	ObjectTypeDB::register_type<VScrollBar>();
	ObjectTypeDB::register_type<ProgressBar>();
	ObjectTypeDB::register_type<HSlider>();
	ObjectTypeDB::register_type<VSlider>();
	ObjectTypeDB::register_type<Popup>();
	ObjectTypeDB::register_type<PopupPanel>();
	ObjectTypeDB::register_type<MenuButton>();
	ObjectTypeDB::register_type<CheckBox>();
	ObjectTypeDB::register_type<CheckButton>();
	ObjectTypeDB::register_type<ToolButton>();
	ObjectTypeDB::register_type<LinkButton>();
	ObjectTypeDB::register_type<Panel>();
	ObjectTypeDB::register_type<Range>();

	OS::get_singleton()->yield(); //may take time to init

	ObjectTypeDB::register_type<TextureFrame>();
	ObjectTypeDB::register_type<Patch9Frame>();
	ObjectTypeDB::register_type<TabContainer>();
	ObjectTypeDB::register_type<Tabs>();
	ObjectTypeDB::register_virtual_type<Separator>();
	ObjectTypeDB::register_type<HSeparator>();
	ObjectTypeDB::register_type<VSeparator>();
	ObjectTypeDB::register_type<TextureButton>();
	ObjectTypeDB::register_type<Container>();
	ObjectTypeDB::register_virtual_type<BoxContainer>();
	ObjectTypeDB::register_type<HBoxContainer>();
	ObjectTypeDB::register_type<VBoxContainer>();
	ObjectTypeDB::register_type<GridContainer>();
	ObjectTypeDB::register_type<CenterContainer>();
	ObjectTypeDB::register_type<ScrollContainer>();
	ObjectTypeDB::register_type<PanelContainer>();
	ObjectTypeDB::register_virtual_type<SplitContainer>();
	ObjectTypeDB::register_type<HSplitContainer>();
	ObjectTypeDB::register_type<VSplitContainer>();

	OS::get_singleton()->yield(); //may take time to init


	ObjectTypeDB::register_virtual_type<ButtonArray>();
	ObjectTypeDB::register_type<HButtonArray>();
	ObjectTypeDB::register_type<VButtonArray>();
	ObjectTypeDB::register_type<TextureProgress>();
	ObjectTypeDB::register_type<ItemList>();

#ifndef	ADVANCED_GUI_DISABLED

	ObjectTypeDB::register_type<FileDialog>();
	ObjectTypeDB::register_type<LineEdit>();
	ObjectTypeDB::register_type<PopupMenu>();
	ObjectTypeDB::register_type<Tree>();

	ObjectTypeDB::register_type<TextEdit>();

	ObjectTypeDB::register_virtual_type<TreeItem>();
	ObjectTypeDB::register_type<OptionButton>();
	ObjectTypeDB::register_type<SpinBox>();
	ObjectTypeDB::register_type<ColorPicker>();
	ObjectTypeDB::register_type<ColorPickerButton>();
	ObjectTypeDB::register_type<RichTextLabel>();
	ObjectTypeDB::register_type<PopupDialog>();
	ObjectTypeDB::register_type<WindowDialog>();
	ObjectTypeDB::register_type<AcceptDialog>();
	ObjectTypeDB::register_type<ConfirmationDialog>();
//	ObjectTypeDB::register_type<VideoPlayer>();
	ObjectTypeDB::register_type<MarginContainer>();

	OS::get_singleton()->yield(); //may take time to init

#endif

	ObjectTypeDB::register_type<World>();
	ObjectTypeDB::register_type<Environment>();
	ObjectTypeDB::register_type<World2D>();
	ObjectTypeDB::register_virtual_type<Texture>();
	ObjectTypeDB::register_type<ImageTexture>();
	ObjectTypeDB::register_type<LargeTexture>();
	ObjectTypeDB::register_virtual_type<Font>();
//	ObjectTypeDB::register_type<BitmapFont>();

	ObjectTypeDB::register_type<DynamicFontData>();
	ObjectTypeDB::register_type<DynamicFont>();

	ObjectTypeDB::register_type<StyleBoxEmpty>();
	ObjectTypeDB::register_type<StyleBoxTexture>();
	ObjectTypeDB::register_type<StyleBoxFlat>();
	ObjectTypeDB::register_type<StyleBoxImageMask>();
	ObjectTypeDB::register_type<Theme>();

	OS::get_singleton()->yield(); //may take time to init


	OS::get_singleton()->yield(); //may take time to init

	ObjectTypeDB::register_type<SceneTree>();

	OS::get_singleton()->yield(); //may take time to init
}

void unregister_scene_types() {

	clear_default_theme();

    memdelete( resource_loader_image );
	memdelete( resource_loader_dynamic_font );

#ifdef TOOLS_ENABLED


#endif


	memdelete( resource_loader_theme );
	memdelete( resource_loader_shader );

//	if (resource_saver_text) {
//		memdelete(resource_saver_text);
//	}
//	if (resource_loader_text) {
//		memdelete(resource_loader_text);
//	}
	SceneStringNames::free();
}
