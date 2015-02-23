/************************************************************************

	Copyright 2012-2014 Luciano Buglioni - Pablo Zitelli

	Contacts:
        Luciano Buglioni: luciano.buglioni@gmail.com
        Pablo Zitelli:    zitelli.pablo@gmail.com
	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    FluxSol is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/
#ifndef _FLUXSOL_H
#define _FLUXSOL_H

//////////////////
// COMMON FILES //
//////////////////

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <limits>

//////////////////
// library file //
//////////////////
#include "Analysis.h"

#include "Boundary.h"


// FIELD
#include "Field.h"
#include "./Field/GeometricField.h"
#include "./Field/FieldOperations.h"
#include "./Field/_CC_Fv_field_Def.h"
#include "./Field/FvField.h"

//////////////////
//Finite Volume //
//////////////////

//Base
#include "./FiniteVolume/FvExp.h"
#include "./FiniteVolume/FvImp.h"
#include "./FiniteVolume/Stencil.h"
//Div
#include "./FiniteVolume/Div/Div.h"
#include "./FiniteVolume/Div/DivScheme.h"
//Grad
#include "./FiniteVolume/Grad/FaceGrad.h"
#include "./FiniteVolume/Grad/FvExpGrad.h"
#include "./FiniteVolume/Grad/FvGradScheme.h"
//Interpolation
#include "./Interpolation/Interpolation_.h"
#include "./Interpolation/CenterToVertexInterpolation.h"
#include "./Interpolation/CenterToVertexInterpolation_Def.h"
#include "./Interpolation/CenterToFaceInterpolation.h"
#include "./Interpolation/CenterToFaceInterpolation_Def.h"
//Laplacian
#include "./FiniteVolume/Laplacian/Laplacian.h"

//Materials
#include "./Materials/Materials.h"

//Mesh
#include "./FiniteVolume/Mesh/CC_Cell.h"
#include "./FiniteVolume/Mesh/FvFace.h"
#include "./FiniteVolume/Mesh/FvGrid.h"

// Input //
#include "./Input/Input.h"
#include "./Input/Read_Field_Def.h"

// Mesh //
#include "Cell.h"
#include "./Mesh/Face.h"
#include "./Mesh/Grid.h"
#include "./Mesh/MeshInfo.h"
#include "./Mesh/Node.h"
#include "./Mesh/Structured.h"
#include "./Mesh/Vertex.h"

#include "Model.h"


// Nastran
#include "./Nastran/Cadenas.h"
#include "./Nastran/Nastran.h"
#include "./Nastran/SistCoord.h"
#include "./Nastran/Varios.h"

//Output
#include "OutputFile.h"

// SistEcuac //
#include "SistEcuac.h"


#include "Solver.h"
#include "SIMPLE.h"

//#include "./Tmp/Tmp.h"
//#include "./Tmp/TmpI.h"

//////////
// Type //
//////////
#include "./Type/Matrix.h"
#include "./Type/Operations.h"
#include "./Type/Products.h"
#include "./Type/pTraits.h"
#include "./Type/Scalar.h"
#include "./Type/Tensor.h"
//#include "./Type/TensorI.h"
#include "./Type/Vec3d.h"
#include "./Type/Vec3dI.h"

#include "./Variables/Variable.h"

#endif
