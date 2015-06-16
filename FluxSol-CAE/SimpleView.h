/*=========================================================================

  Program:   Visualization Toolkit
  Module:    SimpleView.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef SimpleView_H
#define SimpleView_H

#include <vtkRenderWindow.h>
#include <vtkVectorText.h>

#include <vtkRenderWindowInteractor.h>
//NEW!! Extracted from Shadows test project
#if VTK_MAJOR_VERSION <= 5
#include <vtkRenderer.h>
#else
#include <vtkOpenGLRenderer.h>
#endif

#include "./Job/JobWorker.h"
#include "./Job/JobSubmitDialog.h"


#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QMainWindow>


#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkQtTreeView.h>
#include <vtkAxesActor.h>

#include <vtkTransform.h>

#include <vtkTextActor.h>
#include <vtkTextProperty.h>

//GRID
#include <vtkGeometryFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGridGeometryFilter.h>

#include <vtkShrinkFilter.h>
#include <vtkProperty.h>
#include <vtkOrientationMarkerWidget.h>

//Qt Includes
#include <QFileDialog>
#include <QSpinBox>
#include <QComboBox>

//Visualization
#include <vtkCamera.h>
#include <vtkRenderer.h>

#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>


#include "vtkSmartPointer.h"
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

//// TEST SLIDER; TO DELETE

#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation2D.h>

#include <QvtkInteractor.h>


// COMMAND AND INTERACTOR
#include <vtkCommand.h>


#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkObjectFactory.h>

#include "FluxSol.h"


// OUTPUT RESULTS

#include <vtkUnstructuredGrid.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkAppendPolyData.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkXMLUnstructuredGridReader.h>

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

#include <vtkXMLReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLStructuredGridReader.h>
#include <vtkXMLRectilinearGridReader.h>
#include <vtkXMLHyperOctreeReader.h>
#include <vtkXMLCompositeDataReader.h>
#include <vtkXMLStructuredGridReader.h>
#include <vtkXMLImageDataReader.h>
#include <vtkDataSetReader.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkRectilinearGrid.h>
#include <vtkHyperOctree.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkStructuredGrid.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkFieldData.h>
#include <vtkCellTypes.h>
#include <vtksys/SystemTools.hxx>

#include <vtkScalarBarActor.h>

#include <vtkLookupTable.h>

#include "GraphicModel.h"

#include "ModelTree.h"
#include "MsgWindow.h"

#include "GridSelection.h"

#include "ui_SimpleView.h"
#include "GraphicModel.h"

#include <vector>

using namespace std;

// Forward Qt class declarations
class Ui_SimpleView;

// Forward VTK class declarations
class vtkQtTableView;


class SimpleView : public QMainWindow
{
  Q_OBJECT


  vtkSmartPointer<vtkOpenGLRenderer> ren;

public:

  // Constructor/Destructor
  SimpleView();
  ~SimpleView();

public slots:

	virtual void slotOpenFile();
	virtual void slotImportIn();
	virtual void slotViewZpos();
	virtual void slotExit();
	virtual void slotImportMesh();
	virtual void slotOpenResults();


	protected:
	    virtual void ImportMesh(const string &filename);
	    virtual void AddMeshToTree(const Fv_CC_Grid &mesh); //To Modify, specify which model and mesh

protected slots:

private:

    bool ThereIsActiveModel;                //Active Model Indicator
  vtkSmartPointer<vtkQtTableView>           TableView;
  QComboBox *comboBox;
  QComboBox *ResultsVarDim;         //Second combo box of results dimension

  bool  IsModelActive;

  vtkSmartPointer<vtkOrientationMarkerWidget> _vtkAxesWidget;      //MUST BE MEMBER

  vtkSmartPointer<vtkAxesActor> _vtkAxes;                           //Orientation Axes
  vtkSmartPointer<vtkAxesActor> _vtkOriginAxes;                     //Origin Axes

//    vtkSmartPointer<vtkQtTreeView>         TableView;

  // Designer form
  Ui_SimpleView *ui;


  //Derived Defined Object
  ModelTreeWidget *ResultsTab;

  vtkSmartPointer<vtkTextActor> textFluxSolTitleActor;

  vtkSmartPointer<MouseInteractorStyle> gridselectionstyle;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
    vtkSmartPointer<vtkAreaPicker> areaPicker;



    //TO MODIFY
    // OBJECTS TO BE MOVED TO IDE
    CFDModel *activemodel;


    //MODEL CAN BE OR NOT TO BE A GRAPHIC MODEL
    //SINCE IT CAn BE LOADED OR RUN FROM AN ArCHIVE (IF IDE IS NOT READY)
    std::vector < CFDModel* > vmodel;    //All models: TO MODIFY, MUST BE INSIDE IDE
    std::vector <JobSubmitDialog *> vjobsubmitdialog;


    int modelcount;

};

#endif // SimpleView_H
