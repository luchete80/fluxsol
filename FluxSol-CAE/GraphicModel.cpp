#include "GraphicModel.h"

using namespace FluxSol;

GraphicCFDModel::GraphicCFDModel(const std::string s):CFDModel(s)
{
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

        //cout << "X: " << p[0] << ", X: " << p[1] << ", Z: "<< p[3]<<endl;

        points->InsertNextPoint(p);
    }
    this->uGrid->SetPoints(points);
    //TO MODIFY, ASSUMING HEXA CELLS

    cout << "Reading Cells ..."<<endl;

      for (int c=0;c<this->mesh.Num_Cells();c++)
      {
        vtkSmartPointer<vtkHexahedron> hex =
        vtkSmartPointer<vtkHexahedron>::New();

        for (int v=0;v<this->mesh.Cell(c).Num_Vertex();v++)
        {
            int vertpos=this->mesh.Cell(c).Vert(v);
            hex->GetPointIds()->SetId(v,vertpos);
        }
        uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

      }

    //for (int c=0;c<this->mesh.Num_Cells();c++)
        //uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

}

GraphicCFDModel::GraphicCFDModel(const Fv_CC_Grid &im)
{
    this->mesh=im;

       this->uGrid=vtkSmartPointer<vtkUnstructuredGrid>::New();
    // Create the points
    this->points = vtkSmartPointer<vtkPoints>::New();

    //cout << "Vertex Count: " << this->mesh.<<endl;

    //cout << "Creating points ..." <<endl;

    for (int v=0;v<this->mesh.Num_Verts();v++)
    {
        double p[3];

        for (int i=0;i<3;i++)
            p[i]=this->mesh.Vertex(v).Comp()[i];

        //cout << "X: " << p[0] << ", X: " << p[1] << ", Z: "<< p[3]<<endl;

        points->InsertNextPoint(p);
    }
    this->uGrid->SetPoints(points);
    //TO MODIFY, ASSUMING HEXA CELLS

    cout << "Reading Cells ..."<<endl;

      for (int c=0;c<this->mesh.Num_Cells();c++)
      {
        vtkSmartPointer<vtkHexahedron> hex =
        vtkSmartPointer<vtkHexahedron>::New();

        for (int v=0;v<this->mesh.Cell(c).Num_Vertex();v++)
        {
            int vertpos=this->mesh.Cell(c).Vert(v);
            hex->GetPointIds()->SetId(v,vertpos);
        }
        uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

      }

    //for (int c=0;c<this->mesh.Num_Cells();c++)
        //uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
}
