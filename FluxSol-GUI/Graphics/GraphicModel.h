#ifndef _GRAPHICMODEL_H_
#define _GRAPHICMODEL_H_

#include "FluxSol.h"

using namespace FluxSol;

class GraphicModel:public FluxSol::Model
{

};

//CAN HAVE A VIEW WHERE IS POINTING

class GraphicCFDModel:public FluxSol::CFDModel
{

    protected:


    //vtkSmartPointer<vtkUnstructuredGrid> uGrid;

    //vtkSmartPointer<vtkDataSet> dataSet;    //dataSet = ReadAnXMLFile<vtkXMLUnstructuredGridReader> (fileName.toStdString().c_str());

    //vtkSmartPointer<vtkPoints> points;

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
       // vtkSmartPointer<vtkUnstructuredGrid> UGrid () const {return this->uGrid;}

        //If we need in a vector:
        GraphicCFDModel(const GraphicCFDModel &gm){}
        operator=(const GraphicCFDModel &gm){}

};

#endif // _GRAPHICMODEL_H_
