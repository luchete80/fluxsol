#include "Laplacian.h"

namespace FluxSol {


template<typename T>
EqnSystem <T> FvImp::Laplacian(_CC_Fv_Field<Scalar> fi,_CC_Fv_Field <T> &VolField)
{
    EqnSystem <T> eqnsys(VolField.Grid());

    _Surf_Fv_Field<Scalar> fisurf(VolField.Grid());

    fisurf=FvExp::Interpolate(fi);

    //TO MODIFY, AT FIRST; VALUES AT BOUNDARY MUST BE EXTRAPOLATED
//    for (int f=0;f<VolField.Grid().Num_Faces();f++)
//    {
//		_FvFace face=VolField.Grid().Face(f);
//		if (!face.Is_Null_Flux_Face())
//		{
//		    if (VolField.Grid().Face(f).Boundaryface())
//            {
//                int pcellid=face.Cell(0);
//                fisurf.Val(f,VolField.Val(pcellid));
//            }
//		}
//    }


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
	T source;
	Scalar ap, an;  //IF FI IS A SCALAR THESE CAN BE SCALARS
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
				ap=-face.Norm_ad()/face.Dist_pn()*fisurf.Val(f);
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
						if (cell.Neighbour(localncell)==ncellid)
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
            //cout <<"source"<<endl;

            ap=-face.Norm_ad()/fabs(face.Dist_pf_LR(0))*fisurf.Val(idface);

//            cout << "Boundary Face "<<idface<<endl;
//            cout << "k Surface Value"<<fisurf.Val(idface).Val()<<endl;
//            cout << "Cell "<< face.Cell(0) << " ap: "<<ap.Val()<<endl;

			if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
			{
                source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;    //THIS IS RHS, WITH SAME SIGN
				//cout <<"created" <<endl;
				eqnsys.Eqn(face.Cell(0)).Ap()+=ap;
				eqnsys.Eqn(face.Cell(0)).Source()+=source;
			}
			//Formerly according to SEZAI
			else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
			{
                //eqnsys.Eqn(face.Cell(0)).Ap()+=ap;

			    //Formerly the BC was THIS
				source=VolField.Boundaryfield().PatchField(p).Val(f)*fisurf.Val(idface);
				eqnsys.Eqn(face.Cell(0)).Source()+=source;
			}
		}

	}
    //cout << "Returning laplacian"<<endl;
    return eqnsys;
}

} //FluxSol

#include "Laplacian.inst"