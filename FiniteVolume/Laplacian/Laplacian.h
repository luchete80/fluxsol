/************************************************************************

	Copyright 2012-2013 Luciano Buglioni

	Contact: luciano.buglioni@gmail.com

	This file is a part of FluxSol

	FluxSol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    Free CFD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For a copy of the GNU General Public License,
    see <http://www.gnu.org/licenses/>.

*************************************************************************/
#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include "./FiniteVolume/FvImp.h"
#include "./Type/Scalar.h"

using namespace std;

namespace FluxSol{

//Esta funcion es para un simpe valor de campo
template<typename T>
EqnSystem <T> Laplacian(_CC_Fv_Field <T> )
{

    //A continuacion se muestra una forma de calculo de divergencia
    //Sum_f (Sf * (ro * U)f * fi(f) )
    //En OpenFoam ro*U es fi


}

//Esta es una constante
template<typename T>
EqnSystem <T> FvImp::Laplacian(_Surf_Fv_Field<Scalar> fi,_CC_Fv_Field <T> &VolField)
{

}
//Otra opcion puede ser tambien colocarle un simple escalar que implique un cmpo constante
//Esta funcion calcula un gradiente en las caras y lo multiplica por el campo escalar
template<typename T>
EqnSystem <T> FvImp::Laplacian(Scalar fi,_CC_Fv_Field <T> &VolField)
{

    //cout << "Defining eqn..."<<endl;
	EqnSystem <T> eqnsys(VolField.Grid());
	//cout << "Eqn created."<<endl;
	//EqnSystem <T> eqnsys(VolField);
	//EqnSystem <T> eqnsys(VolField.Grid().Num_Cells());   //Como no le doy parametros inicia todo en cero, salvo las dimensiones

    //A continuacion se muestra una forma de calculo de divergencia
    //Sum_f (Sf * (ro * U)f * fi(f) )
    //En OpenFoam ro*U es fi
    //Esta forma de calculo es para cuando las partes de la cara no son iguales
    //--------------
    //look through faces
    //Aca tengo que ensamblar para los dos lados esto segun cada cell con el signo de la face que corresponda
    //eqn=FaceGrad (VolField, f);
	T ap,an,source;
	vector <int> nbr_eqn;

	//Internal field
	//cout << "Face Number"<<VolField.Grid().Num_Faces()<<endl;
	//cout << "Cell Number"<<VolField.Grid().Num_Cells()<<endl;
	for (int f=0;f<VolField.Grid().Num_Faces();f++)
	{
	    //cout << "Face "<<f<<endl;
		_FvFace face=VolField.Grid().Face(f);
		if (!face.Is_Null_Flux_Face())
		{


			if (!VolField.Grid().Face(f).Boundaryface())
			{
			    //cout << "Not boundary face"<<endl;
				ap=-face.Norm_ad()/face.Dist_pn()*fi;
				an=-ap;
				//nbr_eqn.push_back(VolField.Grid().Cell(c));
				//eqnsys.Eqn(face.Cell(0)).Coeffs(ap,an);

				//cout << "Look through neighbours"<<endl;
				for (int nb=0;nb<2;nb++)
				{
				    //cout << "Neighbour" <<endl;
					int localnid;	//Id of local neigbour
					if (nb==0)localnid=1;	else localnid=0;
					//p cell
					int pcellid=face.Cell(nb);
					int ncellid=face.Cell(localnid);
					//cout << "Eqn size" << eqnsys.Num_Eqn()<<endl;
                    //cout << "Creating Eqn ... pcell id"<< pcellid<<endl;

					eqnsys.Eqn(pcellid).Ap()+=ap;
					//cout << "Created" <<endl;
					int neigbour_cell;
					int localneighbourid;	//local cell neigbour
					//Neighbours are
					//Find the global cell
					Cell_CC cell=VolField.Grid().Cell(face.Cell(nb));
					for (int localncell=0;localncell<cell.Num_Neighbours();localncell++)
					{
					    //cout << "Local cell" <<localncell<<endl;
						if (cell.Neigbour(localncell)==ncellid)
							localneighbourid=localncell;
					}
					eqnsys.Eqn(pcellid).An(localneighbourid)+=an;
				}
			}

		}//End if !NullFluxFace

	}//End look trough faces

	// BORDE - BOUNDARY
	//cout << "Laplacian, look through boundary.."<<endl;
	for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
	{
		for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
		{
			int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
			_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
			//Boundary type
			//Instead of if sentence it is convenient to use inheritance
			if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
			{
			    //cout <<"source"<<endl;
				ap=-face.Norm_ad()/fabs(face.Dist_pf_LR(0))*fi;
				source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;
				//cout <<"created" <<endl;
				eqnsys.Eqn(face.Cell(0)).Ap()+=ap;
				eqnsys.Eqn(face.Cell(0)).Source()+=source;
			}
			else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
			{
				source=VolField.Boundaryfield().PatchField(p).Val(f)*fi;
				eqnsys.Eqn(face.Cell(0)).Source()+=source;
			}
		}

	}
    //cout << "Returning laplacian"<<endl;
    return eqnsys;
}

};	//Fin namespace FluxSol

#endif
