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



#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QMainWindow>

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

	protected:

protected slots:

private:

  vtkSmartPointer<vtkQtTableView>         TableView;

//    vtkSmartPointer<vtkQtTreeView>         TableView;

  // Designer form
  Ui_SimpleView *ui;
};

#endif // SimpleView_H
