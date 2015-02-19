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

        const vtkSmartPointer<vtkUnstructuredGrid> UGrid () const {return this->uGrid;}

};
