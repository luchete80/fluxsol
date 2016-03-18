#include "GraphicModel.h"

using namespace FluxSol;

GraphicCFDModel::GraphicCFDModel(const std::string s):CFDModel(s)
{

    ittime_end = clock();

    this->uGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
    // Create the points
    this->points = vtkSmartPointer<vtkPoints>::New();

    //cout << "Vertex Count: " << this->mesh.<<endl;

    cout << "Creating points ..." <<endl;

    for (int v=0;v<this->mesh.Num_Verts();v++)
    {
        double p[3];

        for (int i=0;i<3;i++)
            p[i]=this->mesh.Vertex(v).Comp()[i];

        points->InsertNextPoint(p);
    }
    this->uGrid->SetPoints(points);
    //TO MODIFY, ASSUMING HEXA CELLS

    cout << "Reading Cells ..."<<endl;

        vtkSmartPointer<vtkHexahedron> hex =
        vtkSmartPointer<vtkHexahedron>::New();

      for (int c=0;c<this->mesh.Num_Cells();c++)
      {

        for (int v=0;v<this->mesh.Cell(c).Num_Vertex();v++)
        {
            int vertpos=this->mesh.Cell(c).Vert(v);
            hex->GetPointIds()->SetId(v,vertpos);
        }
        uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

      }

}

GraphicCFDModel::GraphicCFDModel(const Fv_CC_Grid &im)
{
    this->mesh=im;

       this->uGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
    // Create the points
    this->points = vtkSmartPointer<vtkPoints>::New();

    //cout << "Vertex Count: " << this->mesh.<<endl;

    //cout << "Creating points ..." <<endl;
    ittime_end = clock();
    for (int v=0;v<this->mesh.Num_Verts();v++)
    {
        double p[3];

        for (int i=0;i<3;i++)
            p[i]=this->mesh.Vertex(v).Comp()[i];

        points->InsertNextPoint(p);
    }
    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "Point Creation Time: " << ittime_spent <<" seconds" <<endl;

    this->uGrid->SetPoints(points);
    //TO MODIFY, ASSUMING HEXA CELLS

    cout << "Reading Cells ..."<<endl;

    vtkSmartPointer<vtkHexahedron> hex =
        vtkSmartPointer<vtkHexahedron>::New();

      for (int c=0;c<this->mesh.Num_Cells();c++)
      {

        for (int v=0;v<this->mesh.Cell(c).Num_Vertex();v++)
        {
            int vertpos=this->mesh.Cell(c).Vert(v);
            hex->GetPointIds()->SetId(v,vertpos);
        }
        uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

      }

}
