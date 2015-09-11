#include "ImportMesh.h"

void SimpleView::ImportMesh(const string &filename)
{

	QString fileName = QString::fromStdString(filename);

    Fv_CC_Grid mesh;

	//Another option is to separate with commas
	if (!fileName.isEmpty())
	{

        std::string extension =
        vtksys::SystemTools::GetFilenameLastExtension(fileName.toStdString().c_str());


	    vtkUnstructuredGrid *ugrid;

        vtkSmartPointer<vtkDataSetMapper> datasetmapper =
        vtkSmartPointer<vtkDataSetMapper>::New();

          vtkSmartPointer<vtkUnstructuredGridGeometryFilter> geometryFilter =
            vtkSmartPointer<vtkUnstructuredGridGeometryFilter>::New();

      vtkSmartPointer<vtkActor> actor =
        vtkSmartPointer<vtkActor>::New();

        vtkSmartPointer<vtkPolyData> polydata;

        if (extension ==".cgns")
        {
            // GraphicCFDModel model(fileName.toStdString());
            cout << "Importing CGNS Mesh: " << fileName.toStdString() <<" ... "<<endl;

         	mesh=Fv_CC_Grid(fileName.toStdString());

         	ui->MsgWin->AddString(std::string("Importing file " + fileName.toStdString() + "\n") );
         	this->ui->MsgWin->AddString(mesh.StrLog());
         	this->ui->MsgWin->AddString("Mesh has been succesfully imported.\n");
         	cout << "Mesh successfully imported ..."<<endl;
         	GraphicCFDModel model(mesh);
            //mesh.Log("Log.txt");

            vtkSmartPointer<vtkGeometryFilter> geometryFilter =
            vtkSmartPointer<vtkGeometryFilter>::New();

            #if VTK_MAJOR_VERSION <= 5
              geometryFilter->SetInput(model.UGrid());
              //surfaceFilter->SetInput(ugrid);
            #else
              geometryFilter->SetInputData(model.UGrid());

            #endif
              geometryFilter->Update();

            polydata= geometryFilter ->GetOutput ();

             vtkSmartPointer<vtkPolyDataMapper> pdmapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();

            #if VTK_MAJOR_VERSION <= 5
              pdmapper->SetInputConnection(polydata->GetProducerPort());
            #else
             pdmapper->SetInputData(polydata);
            #endif

          ///////////////////////// RENDERING ////

            actor->SetMapper(pdmapper);

        }
        else if (extension==".vtk")
        {
            // ------- SIMPLE READER

              //read all the data from the file
          vtkSmartPointer<vtkXMLUnstructuredGridReader> ugreader =
            vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
          ugreader->SetFileName(fileName.toStdString().c_str());
          ugreader->Update();

          geometryFilter->SetInputConnection(ugreader->GetOutputPort());
          geometryFilter->Update();

          datasetmapper ->SetInputConnection(ugreader->GetOutputPort());
            actor->SetMapper(datasetmapper);
        }

      areaPicker =
        vtkSmartPointer<vtkAreaPicker>::New();

        renderWindowInteractor->SetPicker(areaPicker);


        this->gridselectionstyle->Data = polydata;
        this->renderWindowInteractor->SetInteractorStyle(this->gridselectionstyle);



//       For vtkInteractorStyleRubberBandPick - use 'r' and left-mouse to draw a selection box used to pick
//      vtkSmartPointer<vtkInteractorStyleRubberBandPick> style =
//        vtkSmartPointer<vtkInteractorStyleRubberBandPick>::New();

/////////////////////////////////
// FACE SELECTION /////////////
//------
//  vtkSmartPointer<HighlightInteractorStyleCells> style =
//    vtkSmartPointer<HighlightInteractorStyleCells>::New();
//    style->SetPolyData(polydata);


//    ////////////////////
//     POINT SELECTION ///
//    ////////////////////



  vtkSmartPointer<HighlightInteractorStylePoints> style =
    vtkSmartPointer<HighlightInteractorStylePoints>::New();


      //style->IdFilter()->SetInputConnection(polydata->GetProducerPort());   //This takes a polydata (in original example takes a pointsource)

        //style->IdFilter()->SetInputConnection(ugrid->GetProducerPort());
        //style->IdFilter()->SetInputData(ugrid);

        cout << "Creating Id Filter"<<endl;
        style->IdFilter()->SetInputData(polydata);

      style->IdFilter()->SetIdsArrayName("ids");
      style->IdFilter()->Update();

      style->SurfaceFilter()->SetInputConnection(style->IdFilter()->GetOutputPort());
      style->SurfaceFilter()->Update();


      style->SurfaceFilter()->SetInputConnection(style->IdFilter()->GetOutputPort());
      style->SurfaceFilter()->Update();

        style->SetPoints(polydata); //All points

    // INPUT POLYDATA NLY FOR VISUALIZATION
  //vtkPolyData* input = surfaceFilter->GetOutput();
    ///// END OF POINT SELECTION //////
    ///////////////////////////////////




      // For vtkInteractorStyleTrackballCamera - use 'p' to pick at the current mouse position
      //vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
      //  vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); //like paraview
      style->SetCurrentRenderer(this->ren);
      renderWindowInteractor->SetInteractorStyle( style );



          actor->GetProperty()->SetEdgeColor(0, 0, 0);
          actor->GetProperty()->EdgeVisibilityOn();

      this->ren->AddActor( actor);


    AddMeshToTree(mesh);

	}

}

void SimpleView::AddMeshToTree(const Fv_CC_Grid &mesh)
{
         // ADD MESH TO TREE
        QList<QTreeWidgetItem*> items = this->ui->ModelTree->findItems("Models",Qt::MatchContains|Qt::MatchRecursive);
        int num = items.count();
        //cout <<"Items found"<<num;

        //addTreeChild(items[0], fileName, "Desc");

        //this->ui->ModelTree->update();
        //items[0]->addChild(itm);

        items = this->ui->ModelTree->findItems("Parts",Qt::MatchExactly);

        //Must be finded as MatchRecursive to find a child item
        QList<QTreeWidgetItem*> clist = this->ui->ModelTree->findItems("Parts", Qt::MatchContains|Qt::MatchRecursive, 0);

        foreach(QTreeWidgetItem* item, clist)
        {
             //cout << item->text(0);
        }


        //TO MODIFY, THIS MUST BE DONE IN TREE MEMBER
        if (clist.count()>0)
        {
            QTreeWidgetItem *partitem ;
            //item = new QTreeWidgetItem(this->ui->ModelTree);
            partitem = new QTreeWidgetItem;
            QIcon meshicon;
            meshicon.addFile(QStringLiteral(":/Icons/GridSelectVertex.png"), QSize(), QIcon::Normal, QIcon::Off);
            partitem->setText( 0, "Part-1" );
            partitem->setIcon(0,meshicon);
            clist[0]->addChild(partitem);

            //Add Mesh Child
            QTreeWidgetItem *partmeshit ;
            partmeshit = new QTreeWidgetItem;
            QIcon meshparticon;
            meshparticon.addFile(QStringLiteral(":/Icons/GridSelectVertex.png"), QSize(), QIcon::Normal, QIcon::Off);
            partmeshit->setText( 0, "Mesh" );
            partmeshit->setIcon(0,meshparticon);
            partitem->addChild(partmeshit);

            for (int p=0;p<mesh.vBoundary().Num_Patches();p++)
            {
                //Add Mesh Child
                QTreeWidgetItem *patchit ;
                patchit = new QTreeWidgetItem;
                //QIcon meshparticon;
                //meshparticon.addFile(QStringLiteral(":/Icons/GridSelectVertex.png"), QSize(), QIcon::Normal, QIcon::Off);
                patchit->setText( 0, QString::fromStdString(mesh.vBoundary().vPatch(p).Name()) );
                //patchit->setIcon(0,meshparticon);
                partmeshit->addChild(patchit);
                //cout << "Found Patch: "<<mesh.vBoundary().vPatch(p).Name()<<endl;
            }
            partitem->setExpanded( true );
            partmeshit->setExpanded( true );
        }
        else
        {
            cout << "Parts does not found..."<<endl;
        }


        //clist[0]->setExpanded( true );

        //items[0]->insertChild(0,itm);
        //items[0]->setText(0,"Changed");

        QTreeView modtreeview(this->ui->ModelTree);


        modtreeview.expandAll();
        modtreeview.show();

        //this->ui->ModelTree->expandAll();

        this->ui->ModelTree->update();
        this->ui->ModelTree->show();

      //ADD PATCHES
}
