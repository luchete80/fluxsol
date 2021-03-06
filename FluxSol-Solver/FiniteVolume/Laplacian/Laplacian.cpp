#include "Laplacian.h"

namespace FluxSol {

//
//template<typename T>
//EqnSystem <T> FvImp::Laplacian(_CC_Fv_Field<Scalar> fi,_CC_Fv_Field <T> &VolField)
//{
//    EqnSystem <T> eqnsys(VolField.Grid());
//
//    _Surf_Fv_Field<Scalar> fisurf(VolField.Grid());
//
//    fisurf=FvExp::Interpolate(fi);
//
//    //TO MODIFY, AT FIRST; VALUES AT BOUNDARY MUST BE EXTRAPOLATED
////    for (int f=0;f<VolField.Grid().Num_Faces();f++)
////    {
////		_FvFace face=VolField.Grid().Face(f);
////		if (!face.Is_Null_Flux_Face())
////		{
////		    if (VolField.Grid().Face(f).Boundaryface())
////            {
////                int pcellid=face.Cell(0);
////                fisurf.Val(f,VolField.Val(pcellid));
////            }
////		}
////    }
//
//
//	//cout << "Eqn created."<<endl;
//	//EqnSystem <T> eqnsys(VolField);
//	//EqnSystem <T> eqnsys(VolField.Grid().Num_Cells());   //Como no le doy parametros inicia todo en cero, salvo las dimensiones
//
//    //A continuacion se muestra una forma de calculo de divergencia
//    //Sum_f (Sf * (ro * U)f * fi(f) )
//    //En OpenFoam ro*U es fi
//    //Esta forma de calculo es para cuando las partes de la cara no son iguales
//    //--------------
//    //look through faces
//    //Aca tengo que ensamblar para los dos lados esto segun cada cell con el signo de la face que corresponda
//    //eqn=FaceGrad (VolField, f);
//	T source;
//	Scalar ap, an;  //IF FI IS A SCALAR THESE CAN BE SCALARS
//	vector <int> nbr_eqn;
//
//	//Internal field
//	//cout << "Face Number"<<VolField.Grid().Num_Faces()<<endl;
//	//cout << "Cell Number"<<VolField.Grid().Num_Cells()<<endl;
//	for (std::set<int>::iterator it=VolField.IntNetFluxFaces().begin(); it!=VolField.IntNetFluxFaces().end(); ++it)
//	//for (int f=0;f<VolField.Grid().Num_Faces();f++)
//	{
//	    //cout << "Face "<<f<<endl;
//		//_FvFace face=VolField.Grid().Face(f);
//		//if (!face.Is_Null_Flux_Face())
//		//{
//
//
//			//if (!VolField.Grid().Face(f).Boundaryface())
//			//{
//			    //cout << "Not boundary face"<<endl;
//				ap=-VolField.Grid().Face(*it).Norm_ad()/VolField.Grid().Face(*it).Dist_pn()*fisurf.Val(*it);
//				an=-ap;
//				//nbr_eqn.push_back(VolField.Grid().Cell(c));
//				//eqnsys.Eqn(face.Cell(0)).Coeffs(ap,an);
//
//				//cout << "Look through neighbours"<<endl;
//				for (int nb=0;nb<2;nb++)
//				{
//				    //cout << "Neighbour" <<endl;
//					int localnid;	//Id of local neigbour
//					if (nb==0)localnid=1;	else localnid=0;
//					//p cell
//					int pcellid=VolField.Grid().Face(*it).Cell(nb);
//					int ncellid=VolField.Grid().Face(*it).Cell(localnid);
//					//cout << "Eqn size" << eqnsys.Num_Eqn()<<endl;
//                    //cout << "Creating Eqn ... pcell id"<< pcellid<<endl;
//
//					eqnsys.Eqn(pcellid).Ap()+=ap;
//					//cout << "Created" <<endl;
//					int neigbour_cell;
//					int localneighbourid;	//local cell neigbour
//					//Neighbours are
//					//Find the global cell
//					Cell_CC cell=VolField.Grid().Cell(VolField.Grid().Face(*it).Cell(nb));
//					for (int localncell=0;localncell<cell.Num_Neighbours();localncell++)
//					{
//					    //cout << "Local cell" <<localncell<<endl;
//						if (cell.Neighbour(localncell)==ncellid)
//							localneighbourid=localncell;
//					}
//					eqnsys.Eqn(pcellid).An(localneighbourid)+=an;
//				}
//			//}
//
//		//}//End if !NullFluxFace
//
//	}//End look trough faces
//
//	// BORDE - BOUNDARY
//	//cout << "Laplacian, look through boundary.."<<endl;
//	_FvFace *face;
//	for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
//	{
//		for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//		{
//			int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
//			//_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
//			face=&VolField.Grid().Face(idface);
//			//Boundary type
//			//Instead of if sentence it is convenient to use inheritance
//            //cout <<"source"<<endl;
//
//            ap=-VolField.Grid().Face(idface).Norm_ad()/fabs(VolField.Grid().Face(idface).Dist_pf_LR(0))*fisurf.Val(idface);
//
////            cout << "Boundary Face "<<idface<<endl;
////            cout << "k Surface Value"<<fisurf.Val(idface).Val()<<endl;
////            cout << "Cell "<< face.Cell(0) << " ap: "<<ap.Val()<<endl;
//
//			if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
//			{
//                source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;    //THIS IS RHS, WITH SAME SIGN
//				//cout <<"created" <<endl;
//				eqnsys.Eqn(face->Cell(0)).Ap()+=ap;
//				eqnsys.Eqn(face->Cell(0)).Source()+=source;
//			}
//			//Formerly according to SEZAI
//			else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
//			{
//                //eqnsys.Eqn(face.Cell(0)).Ap()+=ap;
//
//			    //Formerly the BC was THIS
//				source=VolField.Boundaryfield().PatchField(p).Val(f)*fisurf.Val(idface);
//				eqnsys.Eqn(face->Cell(0)).Source()+=source;
//			}
//		}
//
//	}
//    //cout << "Returning laplacian"<<endl;
//    return eqnsys;
//}


// NEW FAST
//
template<typename T>
EqnSystem <T> FvImp::Laplacian(_CC_Fv_Field<Scalar> fi,_CC_Fv_Field <T> &VolField)
{
    EqnSystem <T> eqnsys(VolField.Grid());

    _Surf_Fv_Field<Scalar> fisurf(VolField.Grid());

    fisurf=FvExp::Interpolate(fi);

	T source;
	Scalar ap, an;  //IF FI IS A SCALAR THESE CAN BE SCALARS
	vector <int> nbr_eqn;

	//Internal field
	//cout << "Face Number"<<VolField.Grid().Num_Faces()<<endl;
	//cout << "Cell Number"<<VolField.Grid().Num_Cells()<<endl;
	for (std::set<int>::iterator it=VolField.IntNetFluxFaces().begin(); it!=VolField.IntNetFluxFaces().end(); ++it)
	{

// NEW LAPLACIAN
        ap=-VolField.Grid().Face(*it).Norm_ad()/VolField.Grid().Face(*it).Dist_pn()*fisurf.Val(*it);
        an=-ap;
        //nbr_eqn.push_back(VolField.Grid().Cell(c));
        //eqnsys.Eqn(face.Cell(0)).Coeffs(ap,an);

        //cout << "Look through neighbours"<<endl;
        for (int nb=0;nb<2;nb++)    //Face cells
        {
            int pcellid=VolField.Grid().Face(*it).Cell(nb);

            eqnsys.Eqn(pcellid).Ap()+=ap;
            eqnsys.Eqn(pcellid).An(VolField.Grid().FaceLocalCellNeighbour(*it)[nb])+=an;
        }


	}//End look trough faces

    // TO MODIFY
	// BORDE - BOUNDARY
	//cout << "Laplacian, look through boundary.."<<endl;
	_FvFace *face;
	for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
	{
        if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
        {
            for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
            {
                int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
                //_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
                //face=&VolField.Grid().Face(idface);
                //Boundary type
                //Instead of if sentence it is convenient to use inheritance
                //cout <<"source"<<endl;

                ap=-VolField.Grid().Face(idface).Norm_ad()/fabs(VolField.Grid().Face(idface).Dist_pf_LR(0))*fisurf.Val(idface);

                source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;    //THIS IS RHS, WITH SAME SIGN
                //cout <<"created" <<endl;
                eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Ap()+=ap;
                eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
            }
        }
			//Formerly according to SEZAI
        else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
        {
            for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
            {
                int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
                //_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
                //face=&VolField.Grid().Face(idface);
                //Boundary type
                //Instead of if sentence it is convenient to use inheritance
                //cout <<"source"<<endl;

                ap=-VolField.Grid().Face(idface).Norm_ad()/fabs(VolField.Grid().Face(idface).Dist_pf_LR(0))*fisurf.Val(idface);

                //eqnsys.Eqn(face.Cell(0)).Ap()+=ap;

                //Formerly the BC was THIS
                source=VolField.Boundaryfield().PatchField(p).Val(f)*fisurf.Val(idface);
                eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
            }
        }


	}
    //cout << "Returning laplacian"<<endl;
    return eqnsys;
}


// FAST NEW LAPLACIAN
template<typename T>
EqnSystem <T> FvImp::Laplacian(Scalar fi,_CC_Fv_Field <T> &VolField)
{

    //cout << "Solving laplacian" <<endl;
    clock_t ittime_begin, ittime_end;
    double ittime_spent;


    ittime_end = clock();

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
	T source;
	Scalar ap, an;  //IF FI IS A SCALAR THESE CAN BE SCALARS
	vector <int> nbr_eqn;

	//Internal field0
	//cout << "Face Number"<<VolField.Grid().Num_Faces()<<endl;
	//cout << "Cell Number"<<VolField.Grid().Num_Cells()<<endl;
	//cout << "Laplacian, sizeof intnetfluxface: "<<VolField.IntNetFluxFaces().size()<<endl;
	set <int> intfaces=VolField.IntNetFluxFaces();
	//for (std::set<int>::iterator it=VolField.IntNetFluxFaces().begin(); it!=VolField.IntNetFluxFaces().end(); ++it)

    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "laplacian field gen "<<ittime_spent <<endl;


    ittime_end = clock();

    int pcellid;
    std::set<int>::iterator it;
	for (it=VolField.IntNetFluxFaces().begin(); it!=VolField.IntNetFluxFaces().end(); ++it)
    {
		//_FvFace face=VolField.Grid().Face(*it);

        ap=-VolField.Grid().Face(*it).Norm_ad()/VolField.Grid().Face(*it).Dist_pn()*fi;

        //cout << "Look through neighbours"<<endl;
        for (int nb=0;nb<2;nb++)    //Face cells
        {
            pcellid=VolField.Grid().Face(*it).Cell(nb);

            eqnsys.Eqn(pcellid).Ap()+=ap;
            eqnsys.Eqn(pcellid).An(VolField.Grid().FaceLocalCellNeighbour(*it)[nb])-=ap;
        }

    }

    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "laplacian interior faces loop "<<ittime_spent <<endl;


    ittime_end = clock();


//	// BORDE - BOUNDARY
//	//cout << "Laplacian, look through boundary.."<<endl;
//	// TO MODIFY: THIS IF MUST BE ONCE PER PATCH, NOT FOR FACE!!!!!
//	// PATCH MUST BE A LIST OF NON NULL FLUX FACES
//	for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
//	{
//		for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//		{
//			int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
//			//_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
//			//cout << "BOUNDARY"<<endl;
//
//            if (!VolField.Grid().Face(idface).Is_Null_Flux_Face())
//            {
//                //Boundary type
//                //Instead of if sentence it is convenient to use inheritance
//                //TO MODIFY!!! CHANGE TO INHERITED PATCHFIELD ACTION!!!!!
//                if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
//                {
//                        //cout <<"FIXED VAL"<<endl;
//                        ap=-VolField.Grid().Face(idface).Norm_ad()/fabs(VolField.Grid().Face(idface).Dist_pf_LR(0))*fi;
//                        source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;
//                        //cout <<"created" <<endl;
//                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Ap()+=ap;
//                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
//
//                }
//                else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
//                {
//
//                        //cout << "FIXED WIDTH"<<endl;
//                        source=VolField.Boundaryfield().PatchField(p).Val(f)*fi;
//                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
//
//                }
//            }//If !NullFluxFace
//		}
//
//	}



	// BORDE - BOUNDARY
  // FASTER
	//cout << "Laplacian, look through boundary.."<<endl;
	// TO MODIFY: THIS IF MUST BE ONCE PER PATCH, NOT FOR FACE!!!!!
	// PATCH MUST BE A LIST OF NON NULL FLUX FACES
	for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
	{
		//for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
		//{
			//int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
			//_FvFace face=VolField.Grid().Face(idface);  //TO MODIFY idface or face pos??
			//cout << "BOUNDARY"<<endl;

            //if (!VolField.Grid().Face(idface).Is_Null_Flux_Face())
            //{
                //Boundary type
                //Instead of if sentence it is convenient to use inheritance
                //TO MODIFY!!! CHANGE TO INHERITED PATCHFIELD ACTION!!!!!
                if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
                {
                    for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
                    {
                         int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
                        //cout <<"FIXED VAL"<<endl;
                        ap=-VolField.Grid().Face(idface).Norm_ad()/fabs(VolField.Grid().Face(idface).Dist_pf_LR(0))*fi;
                        source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;
                        //cout <<"created" <<endl;
                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Ap()+=ap;
                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
                    }
                }
                else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
                {
                    for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
                    {
                         int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
                        //cout << "FIXED WIDTH"<<endl;
                        source=VolField.Boundaryfield().PatchField(p).Val(f)*fi;
                        eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
                    }
                }
//                else if (VolField.Boundaryfield().PatchField(p).Type()==NULL_FLUX)
//                {
//                    cout << "Patch passed"<<endl;
//                }
            //}//If !NullFluxFace
		//}

	}
    //cout << "Returning laplacian"<<endl;
    ittime_spent = (double)(clock() - ittime_end) / CLOCKS_PER_SEC;
    ittime_end = clock();
    cout << "laplacian boundary faces loop "<<ittime_spent <<endl;

    return eqnsys;
}

// AS
// NEW FAST NON ORTHOGONAL
//
// FAST NEW LAPLACIAN
template<typename T>
EqnSystem <T> FvImp::NonOrthLaplacian(Scalar fi,_CC_Fv_Field <T> &VolField)
{

    EqnSystem <T> eqnsys(VolField.Grid());
    eqnsys=Laplacian(fi,VolField);

    _CC_Fv_Field<T> right(VolField.Grid());

    //Now Add Deferred Correction Values
    // Fed= fi Se (dfi/dpsi) + fi Se [ dfi/dn - dfi/dpsi ]_old
    //First term is implicit and second explicit
    //fi Se [ dfi/dn - dfi/dpsi ]_old
    //second term of here is like orth face gradient
    //INTERIOR FACES
    Scalar ap,source;
    int pcellid;
    std::set<int>::iterator it;
	for (it=VolField.IntNetFluxFaces().begin(); it!=VolField.IntNetFluxFaces().end(); ++it)
    {
		//_FvFace face=VolField.Grid().Face(*it);
        //fiN - fiP
        //Psi direction
        ap=-VolField.Grid().Face(*it).Norm_ad()/VolField.Grid().Face(*it).Dist_pn()*fi;

        //cout << "Look through neighbours"<<endl;
        for (int nb=0;nb<2;nb++)    //Face cells
        {
            pcellid=VolField.Grid().Face(*it).Cell(nb);
            right[pcellid]=VolField.PrevVal(pcellid)*ap;
            //eqnsys.Eqn(pcellid).Ap()+=ap;
            //eqnsys.Eqn(pcellid).An(VolField.Grid().FaceLocalCellNeighbour(*it)[nb])-=ap;
        }
        //eqnsys==fi * ( VolField.PrevVal().);
    }//End interior faces

    //BOUNDARY
//    for (int p=0;p<VolField.Grid().vBoundary().Num_Patches();p++)
//	{
//
//        if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDVALUE)
//        {
//            for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//            {
//                 int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
//                //cout <<"FIXED VAL"<<endl;
//                ap=-VolField.Grid().Face(idface).Norm_ad()/fabs(VolField.Grid().Face(idface).Dist_pf_LR(0))*fi;
//                source=VolField.Boundaryfield().PatchField(p).Val(f)*ap;
//                //cout <<"created" <<endl;
//                //eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Ap()+=ap;
//                //eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
//            }
//        }
//        else if (VolField.Boundaryfield().PatchField(p).Type()==FIXEDGRADIENT)
//        {
//            for (int f=0;f<VolField.Grid().vBoundary().vPatch(p).Num_Faces();f++)
//            {
//                 int idface=VolField.Grid().vBoundary().vPatch(p).Id_Face(f);
//                //cout << "FIXED WIDTH"<<endl;
//                //source=VolField.Boundaryfield().PatchField(p).Val(f)*fi;
//                //eqnsys.Eqn(VolField.Grid().Face(idface).Cell(0)).Source()+=source;
//            }
//        }
//
//	}//End Patches

    //Apply deferred corrections
    eqnsys==right;
    return eqnsys;
}

} //FluxSol

#include "Laplacian.inst"
