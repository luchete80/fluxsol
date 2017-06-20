// Hello,

// We want to render multiple renderscreen with help of different camera angles and save them in different vtkTextures. We managed it, to render them and save them on the harddisk. But in the moment we render them, and give these rendered screens different vtkTextures, the last in vtkTextures rendered screen, override all other prerendered vtkTextures. For the renderings we use the class vtkWindowToImageFilter. Some code:

// thanks
// maxim

// code:
//x,y,z direction of view
vtkTexture* getImageFromWindow(vtkRenderer *ren,vtkRenderWindow *win, vtkActor *ac, double x, double y, double z)
{
vtkCamera *c= vtkCamera::New();
double *richt = ac->GetCenter();
c->SetPosition(richt[0],richt[1],richt[2]);
c->SetFocalPoint(richt[0]+x,richt[1]+y,richt[2]+z);
ren->SetActiveCamera(c);

win->Render();

vtkWindowToImageFilter *wti= vtkWindowToImageFilter::New();
wti->SetInput(win);

vtkTexture *t = vtkTexture::New();
t->SetInput( wti->GetOutput());
t->InterpolateOn();
return t;
}

//use of the function:
vtkTexture *id2 = vtkTexture::New();
id2 = getImageFromWindow(renderer,renWin,actor,1,0,0);
vtkTexture *id1 = vtkTexture::New();
id1 = getImageFromWindow(renderer,renWin,actor,-1,0,0);

actor->SetTexture(id2);

//actor don't get the texture from id2, but the actor get the texture from id1. although id2 was rendered first and was given to the actor.
thanks for any suggestions.
maxim
-- 
Psssst! Schon vom neuen GMX MultiMessenger gehört? Der kann`s mit allen: http://www.gmx.net/de/go/multimessenger01