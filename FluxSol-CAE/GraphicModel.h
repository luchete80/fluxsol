#include "FluxSol.h"
#include <vtkVersion.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkHexahedron.h>
#include <vtkUnstructuredGrid.h>

#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

class GraphicModel:public Model
{

};

class GraphicCFDModel:public CFDModel
{

    vtkSmartPointer<vtkUnstructuredGrid> uGrid;
    vtkSmartPointer<vtkPoints> points;

    public:
        GraphicCFDModel(const std::string s);
};
