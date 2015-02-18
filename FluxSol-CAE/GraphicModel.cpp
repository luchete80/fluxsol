#include "GraphicModel.h"

GraphicCFDModel::GraphicCFDModel(const std::string s):CFDModel(s)
{
    // Create the points
    this->points = vtkSmartPointer<vtkPoints>::New();

    for (int v=0;v<this->mesh.Num_Verts();v++)
    {
        double p[3];

        points->InsertNextPoint(p);
    }
    this->uGrid->SetPoints(points);
    //TO MODIFY, ASSUMING HEXA CELLS
      for (int c=0;c<this->mesh.Num_Cells();c++)
      {
        vtkSmartPointer<vtkHexahedron> hex =
        vtkSmartPointer<vtkHexahedron>::New();

        //hex->GetPointIds()->SetId(0,0);
        //hex->GetPointIds()->SetId(1,1);
      }

    //for (int c=0;c<this->mesh.Num_Cells();c++)
        //uGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());

}
