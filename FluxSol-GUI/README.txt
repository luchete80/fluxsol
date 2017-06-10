How to render in an IMGUIWindow?
https://github.com/ocornut/imgui/issues/984

ocornut commented on Jan 17
You can:

1- Render your scene in a texture and then render the texture in imgui. For the second part read about comments related to ImTextureId and check the code in your imgui_impl_xxxx.cpp renderer. Check the Image() functions, and ImDrawList::AddImage() etc.

2- Add a Callback to the current window ImDrawList and perform your full rendering within this callback at the right point during rendering.

The first one is simple and convenient and this is what most people would do.
The second one can be useful if you need to save 1 render pass but it is rarely a real problem.
 :+1: 1  
