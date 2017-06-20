
// Constructor
SimpleView::SimpleView()
{

  this-> ren =
    vtkSmartPointer<vtkOpenGLRenderer>::New();
    //vtkSmartPointer<vtkRenderer>::New();
//#endif


    // WINDOW AND INTERACTOR
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(ren);



  this->renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();



   // Add Actor to renderer

 renderWindow->Render();	//If i want to obtain coordinates must to activate renderwindows with Render()



  vtkSmartPointer<vtkCallbackCommand> modifiedCallback =
    vtkSmartPointer<vtkCallbackCommand>::New();
  modifiedCallback->SetCallback (CameraModifiedCallback);

  ren->GetActiveCamera()->AddObserver(vtkCommand::ModifiedEvent,modifiedCallback);


  // VTK/Qt wedded
  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);	//Add

  //This is like the example Four Pane Viewer
  renderWindowInteractor = this->ui->qvtkWidget->GetInteractor();


  //ren->ResetCamera();
  //renderWindow->Render();

    vtkSmartPointer<vtkImageInteractionCallback1> callback =
    vtkSmartPointer<vtkImageInteractionCallback1>::New();

    vtkSmartPointer<vtkWindowsModInteractionCallback> callbackwinmod =
    vtkSmartPointer<vtkWindowsModInteractionCallback>::New();

	callback->SetRenderer(this->ren);

	//renderWindowInteractor->AddObserver( vtkCommand::MouseMoveEvent, callback );

	this->ui->qvtkWidget->GetRenderWindow()->AddObserver( vtkCommand::ModifiedEvent, callbackwinmod );

	//this->ui->qvtkWidget->GetRenderWindow()->AddObserver(vtkCommand::ModifiedEvent,m_pModifiedCallback);

    callbackwinmod->SetRenderer(this->ren);
    callbackwinmod->SetTextActor(this->textFluxSolTitleActor);


	//TO MODIFY
	modelcount=0;
	
	
}