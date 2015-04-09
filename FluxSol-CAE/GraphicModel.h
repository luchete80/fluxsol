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

using namespace FluxSol;

class GraphicModel:public FluxSol::Model
{

};

class GraphicCFDModel:public FluxSol::CFDModel
{

    protected:

    vtkSmartPointer<vtkUnstructuredGrid> uGrid;

    vtkSmartPointer<vtkDataSet> dataSet;    //dataSet = ReadAnXMLFile<vtkXMLUnstructuredGridReader> (fileName.toStdString().c_str());

    vtkSmartPointer<vtkPoints> points;

    Vec3D Dimension;                   //Mesh Dimension
    Vec3D MinCoords;                   //Mesh Dimension
    Vec3D MaxCoords;                   //Mesh Dimension
    Vec3D Center;                       //Mesh center

    double bounds[6];                   //Mesh Bounds as VTK output
    double center[3];

    public:

        GraphicCFDModel(const std::string s);
        GraphicCFDModel(){};
        GraphicCFDModel(const Fv_CC_Grid &im);  //TO MODIFY, INHERIT FROM MODEL
        vtkSmartPointer<vtkUnstructuredGrid> UGrid () const {return this->uGrid;}

};
