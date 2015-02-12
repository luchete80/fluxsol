#include "GraphicModel.h"

GraphicCFDModel(const std::string s):CFDModel(s)
{
  // Create the points
  points = vtkSmartPointer<vtkPoints>::New();


//  for (int c=0;c<this->Mesh)
  {
    vtkSmartPointer<vtkHexahedron> hex =
    vtkSmartPointer<vtkHexahedron>::New();
  }



}
